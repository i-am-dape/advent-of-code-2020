package main

import "bufio"
import "fmt"
import "os"


func processStdin() int {
	scanner := bufio.NewScanner(os.Stdin)
	cnt := 0
	col := 0
	if !scanner.Scan() {
		fmt.Println("unexpected scan stop at first line")
		return -1
	}

	row := 0
	for scanner.Scan() {
		col += 3
		row++
		txt := scanner.Text()

		cur := txt[col % len(txt)]
		fmt.Println(row, col, col % len(txt), string(cur), len(txt))

		if cur == '#' {
			cnt++
		}
	}
	return cnt
}


func main() {		
	fmt.Println(processStdin())
}
