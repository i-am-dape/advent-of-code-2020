package main

import "bufio"
import "fmt"
import "os"
import "sort"
//import "strings"

func move(min, max int, c, up, down rune) (int, int) {
	delta := max - min
	if delta == 1 {
		switch c {
		case up:
			return max, max
		case down:
			return min, min
		}
	}

	mid := ((max - min) / 2) + min	
	switch c {
	case down:
		return min, mid
		
	case up:
		return mid+1, max

	default:
		panic("unknown move")
	}
}

func decodeSeatCode(txt string) (int, int) {
	row := 0
	col := 0
	min := 0
	max := 127

	for _, c := range txt[:7] {		
		//fmt.Println(min, max, string(c))
		min, max = move(min, max, c, 'B', 'F')
	}
	row = min

	min = 0
	max = 7
	for _, c := range txt[7:] {				
		//fmt.Println(min, max, string(c))
		min, max = move(min, max, c, 'R', 'L')
	}
	col = min
	return row, col
}

func processPass(txt string) (int, int, int) {
	row, col := decodeSeatCode(txt)
	id := (row * 8) + col
	//fmt.Println(row, col, id)
	return row, col, id
}


func processStdin() {
	scanner := bufio.NewScanner(os.Stdin)
	ids := []int{}
		
	for scanner.Scan() {		
		_, _, id := processPass(scanner.Text())
		ids = append(ids, id)
	}

	sort.Ints(ids)
	fmt.Println(ids)

	p := 0
	for _, c := range ids {
		if p == (c - 2) {
			fmt.Println(p, c)
			return
		}
		p = c
	}	
}

func main() {
	fmt.Println("5")
	processStdin()

	//fmt.Println(processPass("FBFBBFFRLR"))
	//fmt.Println(processPass("BFFFBBFRRR"))
	//fmt.Println(processPass("FFFBBBFRRR"))
	//fmt.Println(processPass("BBFFBBFRLL"))
}