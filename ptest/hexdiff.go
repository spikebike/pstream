package main

import (
    "bufio"
    "fmt"
    "os"
    "strconv"
)

func main() {
    if len(os.Args) != 2 {
        fmt.Println("Usage: go run hex_diff.go <filename>")
        return
    }

    filename := os.Args[1]
    file, err := os.Open(filename)
    if err != nil {
        fmt.Println("Error opening file:", err)
        return
    }
    defer file.Close()

    scanner := bufio.NewScanner(file)
    var prev uint64

    for scanner.Scan() {
        hexStr := scanner.Text()[2:] // Strip the "0x" prefix
        curr, err := strconv.ParseUint(hexStr, 16, 64) // Parse as 64-bit integer
        if err != nil {
            fmt.Println("Error parsing hex:", err)
            continue
        }

        if prev != 0 { // Skip the very first line
            diff := int64(curr) - int64(prev) // Handle potential negatives
            fmt.Println("Difference:", diff)
        }

        prev = curr
    }

    if err := scanner.Err(); err != nil {
        fmt.Println("Error reading file:", err)
    }
}

