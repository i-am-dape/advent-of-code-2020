package main

import (
	"bufio"
	"fmt"
	"os"
)

//import "strings"

func processStdin() int {
	scanner := bufio.NewScanner(os.Stdin)
	tmp := map[rune]interface{}{}
	sum := 0

	for scanner.Scan() {
		if len(scanner.Text()) == 0 {
			sum += len(tmp)
			fmt.Println(sum, len(tmp))
			tmp = map[rune]interface{}{}
			fmt.Println("summarizing")
			continue
		}

		if scanner.Text() == "###" {
			fmt.Println("### end")
			break
		}

		for _, c := range scanner.Text() {
			tmp[c] = 0
		}
		fmt.Println("processed", scanner.Text())
	}

	fmt.Println(tmp, len(tmp))

	return sum
}

func main() {
	fmt.Println("6")
	fmt.Println(processStdin())
}
