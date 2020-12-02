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

func main() {
	in, err := readStdin()
	if err != nil {
		fmt.Println("bad input; failed")
		return
	}

	cnt := 0
	for i := 0; i < len(in); i++ {
		cur := in[i]
		for j := len(in) - 1; j > i; j-- {
			rev := in[j]

			cnt++
			tot := cur + rev
			if tot == 2020 {
				fmt.Println("match", i, cur, j, rev, tot, cur * rev)
			} else if tot < 2020 {
				fmt.Println("skip", i, cur, j, rev, tot)
				break
			} else if j % 10 == 0 {
				//fmt.Println(i, cur, j, rev, tot)
			}
		}
	}

	fmt.Println("done", cnt)
}

