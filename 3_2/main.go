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

	fmt.Println("map:", len(m), "x", len(m[0]))
	next := down

	for n, txt := range m {		
		if n != next {
			continue
		}
		
		col += right

		cur := txt[col % len(txt)]

		if cur == '#' {
			cnt++
		}

		next += down
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

	fmt.Println(31, 31 % 1, 31 % 3, 31 % 5, 31 % 7, 31 % 1)
	fmt.Println(66, 66 % 1, 66 % 3, 66 % 5, 66 % 7, 66 % 1)
	fmt.Println(323 % 2)

	fmt.Println(r1d1, r3d1, r5d1, r7d1, r1d2)
	fmt.Println(r1d1 * r3d1 * r5d1 * r7d1 * r1d2)
}
