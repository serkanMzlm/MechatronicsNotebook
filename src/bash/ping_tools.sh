#!/bin/bash

NC='\033[0m'         # No Color
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'

TOTAL_CHECKED=0
TOTAL_ONLINE=0
TOTAL_OFFLINE=0

cleanup_and_exit() {
    echo
    echo "================================"
    echo "Operation interrupted by user"
    
    if [ $TOTAL_CHECKED -gt 0 ]; then
        echo "================================"
        echo "Checked: $TOTAL_CHECKED IPs"
        echo "Online: $TOTAL_ONLINE"
        echo "Offline: $TOTAL_OFFLINE"
        echo "================================"
    fi
    
    echo "Exiting..."
    exit 130  # 128 + 2 (SIGINT)
}

trap cleanup_and_exit SIGINT SIGTERM

ping_single_ip() {
    local ip=$1
    
    if ping -c 1 -W 1 "$ip" &> /dev/null; then
        echo -e "${GREEN}✓${NC} $ip is ${GREEN}ONLINE${NC}"
        return 0
    else
        echo -e "${RED}✗${NC} $ip is ${RED}OFFLINE${NC}"
        return 1
    fi
}

ping_from_file() {
    local file=$1
    
    if [ ! -f "$file" ]; then
        echo -e "${RED}Error: File '$file' not found!${NC}"
        return 1
    fi
    
    echo -e "${BLUE}Reading IPs from file: $file${NC}"
    echo "================================"
    
    TOTAL_CHECKED=0
    TOTAL_ONLINE=0
    TOTAL_OFFLINE=0
    
    while IFS= read -r ip; do
        # Yorum satırlarını ve boş satırları atla
        [[ -z "$ip" || "$ip" =~ ^[[:space:]]*# ]] && continue
        
        ((TOTAL_CHECKED++))
        
        if ping_single_ip "$ip"; then
            ((TOTAL_ONLINE++))
        else
            ((TOTAL_OFFLINE++))
        fi
        
    done < "$file"
    
    echo "================================"
    echo -e "Total: $TOTAL_CHECKED | ${GREEN}Online: $TOTAL_ONLINE${NC} | ${RED}Offline: $TOTAL_OFFLINE${NC}"
}

ping_ip_range() {
    local start_ip=$1
    local end_ip=$2
    
    local start_last=$(echo "$start_ip" | cut -d'.' -f4)
    local end_last=$(echo "$end_ip" | cut -d'.' -f4)
    local network=$(echo "$start_ip" | cut -d'.' -f1-3)
    local end_network=$(echo "$end_ip" | cut -d'.' -f1-3)
    
    if [ "$network" != "$end_network" ]; then
        echo -e "${RED}Error: IPs must be in the same subnet!${NC}"
        echo "Example: 192.168.1.1 to 192.168.1.50"
        return 1
    fi
    
    echo -e "${BLUE}Scanning IP range: $start_ip - $end_ip${NC}"
    echo "================================"
    
    TOTAL_CHECKED=0
    TOTAL_ONLINE=0
    TOTAL_OFFLINE=0
    
    for ((i=start_last; i<=end_last; i++)); do
        local current_ip="${network}.${i}"
        ((TOTAL_CHECKED++))
        
        if ping_single_ip "$current_ip"; then
            ((TOTAL_ONLINE++))
        else
            ((TOTAL_OFFLINE++))
        fi
    done
    
    echo "================================"
    echo -e "Total: $TOTAL_CHECKED | ${GREEN}Online: $TOTAL_ONLINE${NC} | ${RED}Offline: $TOTAL_OFFLINE${NC}"
}

ping_subnet() {
    local cidr=$1
    
    local ip=$(echo "$cidr" | cut -d'/' -f1)
    local mask=$(echo "$cidr" | cut -d'/' -f2)
    
    if [ "$mask" -lt 24 ] || [ "$mask" -gt 28 ]; then
        echo -e "${RED}Error: Only /24 to /28 subnet masks are supported${NC}"
        return 1
    fi
    
    local network=$(echo "$ip" | cut -d'.' -f1-3)
    
    local host_count=$((2 ** (32 - mask) - 2))  # Network ve broadcast hariç
    
    echo -e "${BLUE}Scanning subnet: $cidr ($host_count hosts)${NC}"
    echo "================================"
    
    TOTAL_CHECKED=0
    TOTAL_ONLINE=0
    TOTAL_OFFLINE=0
    
    local start=1
    local end=254
    
    if [ "$mask" -eq 25 ]; then
        end=126
    elif [ "$mask" -eq 26 ]; then
        end=62
    elif [ "$mask" -eq 27 ]; then
        end=30
    elif [ "$mask" -eq 28 ]; then
        end=14
    fi
    
    for ((i=start; i<=end; i++)); do
        local current_ip="${network}.${i}"
        ((TOTAL_CHECKED++))
        
        if ping_single_ip "$current_ip"; then
            ((TOTAL_ONLINE++))
        else
            ((TOTAL_OFFLINE++))
        fi
    done
    
    echo "================================"
    echo -e "Total: $TOTAL_CHECKED | ${GREEN}Online: $TOTAL_ONLINE${NC} | ${RED}Offline: $TOTAL_OFFLINE${NC}"
}

# IP formatını kontrol et
is_valid_ip() {
    local ip=$1
    
    if [[ $ip =~ ^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$ ]]; then
        return 0
    else
        return 1
    fi
}

show_usage() {
    echo "Usage: $0 [OPTIONS]"
    echo
    echo "Options:"
    echo "  -i <IP>              Ping single IP address"
    echo "  -f <FILE>            Ping IPs from file (one IP per line)"
    echo "  -r <START> <END>     Ping IP range"
    echo "  -s <CIDR>            Ping subnet (CIDR notation)"
    echo "  -h                   Show this help message"
    echo
    echo "Examples:"
    echo "  $0 -i 192.168.1.1"
    echo "  $0 -f ip_list.txt"
    echo "  $0 -r 192.168.1.1 192.168.1.50"
    echo "  $0 -s 192.168.1.0/24"
}

main() {
    if [ $# -eq 0 ]; then
        show_usage
        exit 1
    fi
    
    case "$1" in
        -i)
            if [ -z "$2" ]; then
                echo -e "${RED}Error: IP address required${NC}"
                show_usage
                exit 1
            fi
            
            if ! is_valid_ip "$2"; then
                echo -e "${RED}Error: Invalid IP address format${NC}"
                exit 1
            fi
            
            ping_single_ip "$2"
            ;;
            
        -f)
            if [ -z "$2" ]; then
                echo -e "${RED}Error: File path required${NC}"
                show_usage
                exit 1
            fi
            
            ping_from_file "$2"
            ;;
            
        -r)
            if [ -z "$2" ] || [ -z "$3" ]; then
                echo -e "${RED}Error: Start and end IP addresses required${NC}"
                show_usage
                exit 1
            fi
            
            if ! is_valid_ip "$2" || ! is_valid_ip "$3"; then
                echo -e "${RED}Error: Invalid IP address format${NC}"
                exit 1
            fi
            
            ping_ip_range "$2" "$3"
            ;;
            
        -s)
            if [ -z "$2" ]; then
                echo -e "${RED}Error: CIDR notation required${NC}"
                show_usage
                exit 1
            fi
            
            ping_subnet "$2"
            ;;
            
        -h|--help)
            show_usage
            exit 0
            ;;
            
        *)
            echo -e "${RED}Error: Invalid option '$1'${NC}"
            show_usage
            exit 1
            ;;
    esac
}

main "$@"