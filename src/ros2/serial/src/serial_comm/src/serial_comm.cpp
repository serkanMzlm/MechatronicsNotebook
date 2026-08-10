#include "serial_comm.hpp"

SerialComm::SerialComm(std::string dev_name): Node("joy_node"),dev_(dev_name){
  initPort();
  msg.axes.resize(5);
  msg.buttons.resize(10);
  memset(buffer, 0, sizeof(buffer));
  joy_pub = this->create_publisher<JoyMsg>("joy", 10);
  timer_ = this->create_wall_timer(std::chrono::milliseconds(),
                                 std::bind(&SerialComm::dataRead, this));

}

SerialComm::~SerialComm(){
    close(port_);
}

bool SerialComm::initPort(){
    port_ = open(dev_.c_str(), O_RDWR);
    if(port_ < 0){
        /**
         * error 2 : Dosyanın bulunamamsı      (No such file or directory)
         * error 13: İzinlerin yetersiz olamsı (Permission denied)
         *           Bu hata durumunda `sudo adduser $USER dialout` ayarların
         *           geçerli olması için oturumun kapatılıp açılması lazım
        */
        std::cout << "Error " << errno << " from open : " << strerror(errno) << std::endl;
        return false;
    }
    if(!configure()){
        std::cout << "Error configure..." << std::endl;
        return false;
    }
    usleep(1000000);
    return true;
}

/**
 * Yapılandırma ayarlarında sadece belirtilen bit değiştirmek için
 * `&=` veya `|=` operatörleri kullanılmalıdır.
 * termios {
 *      tcflag_t c_iflag   giriş modu flag
 *      tcflag_t c_oflag   çıkış modu flag
 *      tcflag_t c_cflag   kontrol modu flag
 *      tcflag_t c_lflag   yerel modu flag
 *      cc_t c_line
 *      cc_t c_cc[NCCS]    kontrol karakteri
 * }
*/
bool SerialComm::configure(){
    struct termios tty;
    if(tcgetattr(port_, &tty) != 0){
        std::cout << "Error " << errno << " from tcgetattr : " << strerror(errno) << std::endl;
        close(port_);
        return false;
    }
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;  
    tty.c_lflag &= ~ECHOE; 
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG; 

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;

    tty.c_cc[VTIME] = 10;  
    tty.c_cc[VMIN] = 1;

    // cfsetispeed(&tty, BAUDRATE);
    cfsetspeed(&tty, BAUDRATE);
    tcflush(port_, TCIFLUSH);
    if (tcsetattr(port_, TCSANOW, &tty) != 0) {
        std::cout << "Error " << errno << " from tcsetattr : " << strerror(errno) << std::endl;
        close(port_);
        return false;
    }

    return true;
}

void SerialComm::dataRead(){
  while (read(port_, &curr_byte, 1) > 0){
#ifdef DEBUG
    int value =  static_cast<int>(curr_byte);
    if (flag_){
    std::cout << "value:" << static_cast<int>(curr_byte) << std::endl;
    }
    if(value == CONTROL_BYTE || FLAG_DISABLE){ flag_ = true; }
    else { flag_ = false; }
#else 
    if(state == 0){
        if(curr_byte == HEADER_ && prev_byte == FOOTHER_){
            buffer[state++] = curr_byte;
        }else{
            state = 0;
        }
    }else if(state < HEADER_LEN + PAYLOAD_LEN){
        buffer[state++] = curr_byte;
    }else if(state < HEADER_LEN + PAYLOAD_LEN + FOOTHER_LEN){
        buffer[state++] = curr_byte;
        state = 0;
        prev_byte = curr_byte;
        if(curr_byte == FOOTHER_){
            msg.axes[4]    = mapValues(static_cast<float>(buffer[1]), 0, 200, -1, 1);
            msg.axes[3]    = mapValues(static_cast<float>(buffer[2]), 0, 200, -1, 1);
            msg.axes[1]    = mapValues(static_cast<float>(buffer[3]), 0, 200, -1, 1);
            msg.axes[0]    = mapValues(static_cast<float>(buffer[4]), 0, 200, -1, 1);

            msg.buttons[1] = static_cast<int>(((buffer[5] == 1) ? 1 : 0));
            msg.buttons[0] = static_cast<int>(((buffer[5] == 2) ? 1 : 0));
            msg.buttons[6] = static_cast<int>(((buffer[5] == 3) ? 1 : 0));
            msg.buttons[7] = static_cast<int>(((buffer[5] == 4) ? 1 : 0));
            joy_pub->publish(msg);
        }else{
            state = 0;
        }
    }else{
        state = 0;
    }
    prev_byte = curr_byte;
#endif
  }
}

int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SerialComm>());
    rclcpp::shutdown();
    return 0;
}