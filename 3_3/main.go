package main

import "bufio"
import "fmt"
import "os"

type Map []string

func processStdin() Map {
	scanner := bufio.NewScanner(os.Stdin)
	m := Map{}
	for scanner.Scan() {
		m = append(m, scanner.Text())
	}
	return m
}

func processMap(m Map, right, down int) int {
	cnt := 0
	col := 0

	m = m[1:]

	for n, txt := range m {
		if n % down != 0 {
			fmt.Println(right, down, n, "skip")
			continue
		}
		//row := n + 1
		col += right

		cur := txt[col % len(txt)]
		//fmt.Println(row, col, col % len(txt), string(cur), len(txt))

		if cur == '#' {
			cnt++
		}
	}

	return cnt
}

func main() {	
	m := processStdin()

	r1d1 := processMap(m, 1, 1)
	r3d1 := processMap(m, 3, 1)
	r5d1 := processMap(m, 5, 1)
	r7d1 := processMap(m, 7, 1)
	r1d2 := processMap(m, 1, 2)

	fmt.Println(r1d1, r3d1, r5d1, r7d1, r1d2)
	fmt.Println(r1d1 * r3d1 * r5d1 * r7d1 * r1d2)
}
