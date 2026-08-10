#!/bin/bash

demo_input_variables() {
        echo "=== User Input and Variable Demo ==="
        echo "Enter your name:"
        read user_name       

        # Ya da promp ile birlikte
        read -p "Enter your name: " user_name

        # Birden fazla değişken
        echo "Enter your city and country:"
        read city country

        echo
        echo "Summary:"
        echo "  Name: $user_name"
        echo "  Location: $city, $country"
}


demo_conditionals() {
        echo "=== Conditionals (if, case) Demo ==="
        read -p "Enter a number (1-100): " number

        if [ $number -lt 33 ]; then
        echo "$number is in lower range (1-32)"
        elif [ $number -lt 67 ]; then
        echo "$number is in middle range (33-66)"
        else
        echo "$number is in upper range (67-100)"
        fi

        echo
        echo "Menu:"
        echo "  1) Show date"
        echo "  2) Show time"
        echo
        read -p "Select option: " option

        case $option in
        1)echo "Date: $(date '+%Y-%m-%d')" ;;
        2)echo "Time: $(date '+%H:%M:%S')" ;;
        *)echo "Invalid option!";;
        esac
}

demo_loop() {
    echo "=== For and While Loop Demo ==="
   
    # Liste ile döngü
    echo "1. Loop with list:"
    for item in apple banana orange grape; do
        echo "  - $item"
    done
    
    # Range kullanarak
    echo "2. Loop with range {1..10}:"
    for i in {1..10}; do
        echo -n "$i "
    done
    echo
       
    # C-style for loop
    echo "4. C-style for loop:"
    for ((i=0; i<5; i++)); do
        echo "  Index: $i"
    done
    
    counter=1
    while [ $counter -le 5 ]; do
        echo "  Count: $counter"
        counter=$((counter + 1))
    done
    
    echo "Type 'quit' to exit:"
    while true; do
        read -p "> " input
        
        if [ "$input" == "quit" ]; then
            echo "Exiting loop..."
            break
        else
            echo "You typed: $input"
        fi
    done
}

demo_functions() {
    echo "=== Functions Demo ==="
    greet() {
        echo "Hello, World!"
    }
    
    greet_person() {
        local name=$1
        local age=$2
        echo "Hello, $name! You are $age years old."
    }
    
    add_bc_used_in_function() {
        local a=$1
        local b=$2
        if command -v bc &> /dev/null; then
            result=$(echo "scale=2; $a / $b" | bc)
            echo $result
        else
            echo "bc command not found!"
        fi
    }
    
    greet
    greet_person "Alice" 30
    
    result=$(add_bc_used_in_function 5 7)
    echo "5 + 7 = $result"
}


demo_strings() {
        echo "=== String Operations Demo ==="
        text="Hello World"

        echo "Text: '$text'"
        echo "Length: ${#text}" # Uzunluk

        # Alt string
        echo "Substring (0,5): '${text:0:5}'"
        echo "Substring (6): '${text:6}'"

        # Büyük/Küçük harf
        echo "Uppercase: '${text^^}'"
        echo "Lowercase: '${text,,}'"

        # String birleştirme
        current_date=$(date "+%Y-%m-%d") # Komut sonucunu değişkene atama
        file_count=$(ls | wc -l)         # Komut sonucunu değişkene atama
        summary="Date: $current_date, Files: $file_count"
        echo "Summary: '$summary'"
}

demo_file_operations() {
    echo "=== File Operations Demo ==="
    test_file="test_file.txt"
    test_dir="test_directory"
    
    # Dosya oluştur
    echo "Creating file: $test_file"
    echo "This is a test file" > $test_file
    
    # Dosya var mı kontrol
    if [ -f $test_file ]; then
        echo "File exists!"
        echo "File size: $(wc -c < $test_file) bytes"
    fi
    
    echo "Creating directory: $test_dir"
    mkdir -p $test_dir
    
    if [ -d $test_dir ]; then  # Dizin var mı kontrol
        echo "Directory exists!"
    fi
    
    read -p "Delete test files? (y/n): " cleanup 
    if [ "$cleanup" == "y" ]; then
        rm -f $test_file
        rm -rf $test_dir
        echo "Cleanup done!"
    fi
}

while true; do
        clear
        echo "1)  User Input and Variable Demo"
        echo "2)  Conditionals (if-else, case) Demo"
        echo "3)  For and While Loop Demo"
        echo "4)  Functions Demo"
        echo "5)  String Operations Demo"
        echo "6)  File Operations Demo"
        echo "7)  Exit"
        echo
        read -p "Select demo (1-6): " choice

        echo

        case $choice in
                1) demo_input_variables ;;
                2) demo_conditionals ;;
                3) demo_loop ;;
                4) demo_functions ;;
                5) demo_strings ;;
                6) demo_file_operations ;;
                7) exit 0 ;;
                *) echo "Invalid choice!" ;;
        esac

        echo 
        read -p "Press Enter to continue..."
done
