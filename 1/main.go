package main

import "bufio"
import "fmt"
import "os"
import "strconv"

func readStdin() ([]int, error) {
	in := []int{}

	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		n, err := strconv.Atoi(scanner.Text())
		if err != nil {
			return nil, err
		}

		in = append(in, n)
	}

	return in, nil
}

func findTerm(sum, left int, terms []int) (term, at int) {
	for j := len(terms) - 1; j >= 0; j-- {
		right := terms[j]

		tot := left + right

		if j % 10 == 0 {
			//fmt.Println(i, cur, j, rev, tot)
		}

		if tot == sum {
			return right, j
		} else if tot < sum {							
			return -1, -1
		}
	}
	return -1, -1
}

func main() {
	in, err := readStdin()
	if err != nil {
		fmt.Println("bad input; failed")
		return
	}

	for i := 0; i < len(in); i++ {
		left := in[i]
		right, at := findTerm(2020, left, in[i+1:])
		if at < 0 {
			continue	
		}

		fmt.Println(i, left, at, right, left + right, left * right)
	}
}

