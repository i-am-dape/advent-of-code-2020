package main

import (
	"bufio"
	"fmt"
	"os"
)

//import "strings"

func processStdin() int {
	scanner := bufio.NewScanner(os.Stdin)
	groups := [][]map[rune]interface{}{}
	group := []map[rune]interface{}{}

	for scanner.Scan() {
		if scanner.Text() == "" {
			groups = append(groups, group)
			group = []map[rune]interface{}{}
			continue
		}

		if scanner.Text() == "###" {
			fmt.Println("### end")
			break
		}

		tmp := map[rune]interface{}{}
		for _, c := range scanner.Text() {
			tmp[c] = 0
		}
		group = append(group, tmp)
		fmt.Println("processed", scanner.Text())
	}

	ga := []map[rune]interface{}{}
	for _, group := range groups {
		all := map[rune]interface{}{}
		for _, member := range group {
			for q := range member {
				all[q] = 0
			}
		}
		ga = append(ga, all)
	}

	cnts := []int{}
	for n, group := range groups {
		all := ga[n]
		unq := 0
		for c := range all {
			cnt := 0
			for _, member := range group {
				if _, ok := member[c]; ok {
					cnt++
				}
			}

			if cnt == len(group) {
				unq++
			}
		}
		cnts = append(cnts, unq)
	}

	fmt.Println(cnts)

	sum := 0
	for _, n := range cnts {
		sum += n
	}

	return sum
}

func main() {
	fmt.Println("6")
	fmt.Println(processStdin())
}
