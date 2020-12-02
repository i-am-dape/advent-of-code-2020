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

func findSub(target, left int, terms []int) []int {
	for i := len(terms) - 1; i >= 0; i-- {
		if left + terms[i] <= target {
			//fmt.Println(terms[i], terms[:i+1])
			return terms[:i+1]
		}
	}
	return nil
}

const target = 2020

func dummy(in []int) {
	cnt := 0

	for kat, k := range in {
		for lat, l := range in {
			if kat == lat {
				continue
			}

			for mat, m := range in {
				cnt++
				if kat == mat || mat == lat {
					continue
				}

				sum := k + l + m
				if sum == 2020 {
					fmt.Println("found!", k, l, m, k + l + m)
					fmt.Println("at", kat, lat, mat)
					fmt.Println(k*l*m)
				}
			}
		}
	}
	fmt.Println(">>>>", cnt)
}

func main() {
	in, err := readStdin()
	if err != nil {
		fmt.Println("bad input; failed")
		return
	}

	dummy(in)
	return

	// This doesn't work... dummy is dum but works...
	for i := 0; i < len(in); i++ {
		left := in[i]
		sub := findSub(target, left, in[i+1:])		
		if len(sub) == 0 {
			fmt.Println("no subrange possible")
			return
		}

		fmt.Println("--------------------------------------")
		fmt.Println("1", left, sub)

		for at, r1 := range sub {
			sub2 := findSub(target, left + r1, sub[at+1:])
			if len(sub2) == 0 {
				continue
			}

			fmt.Println("2:", left + r1, sub2)
			//return

			for _, r2 := range sub2 {
				sum := left + r1 + r2
				//fmt.Println(left, r1, r2, sum)
				if sum == target {
					fmt.Println(left, r1, r2, sum, left * r1 * r2)
				}
			}
		}

		//fmt.Println(sub)
	}
}

