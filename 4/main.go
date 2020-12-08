package main

import "bufio"
import "fmt"
import "os"
import "strings"

type passport map[string]string

var fields = map[string]bool {
	"byr": true,
	"iyr": true,
	"eyr": true,
	"hgt": true,
	"hcl": true,
	"ecl": true,
	"pid": true,
	"cid": false,
}

func parse(scanner *bufio.Scanner) passport {
	p := passport{}

	for {
		fmt.Println(scanner.Text())
		parts := strings.Split(scanner.Text(), " ")
		fmt.Println(parts, len(parts))
		for _, part := range parts {
			kv := strings.Split(part, ":")
			p[kv[0]] = kv[1]
		}

		if !scanner.Scan() {			
			return p
		}

		if scanner.Text() == "" {
			fmt.Println("")
			return p
		}
	}
}

func processStdin() []passport {
	scanner := bufio.NewScanner(os.Stdin)
	passports := []passport{}
	
	for scanner.Scan() {
		passport := parse(scanner)
		fmt.Println(passport)
		passports = append(passports, passport)
	}
	return passports
}

func findMissing(p passport) map[string]interface{} {
	m := map[string]interface{}{}
	for field, req := range fields {
		if _, ok := p[field]; !ok && req {
			m[field] = 0
		}
	}

	return m
}

func main() {
	fmt.Println("4")
	passports := processStdin()
	valid := 0
	invalid := 0
	for _, passport := range passports {
		missing := findMissing(passport)
		if len(missing) == 0 {
			valid++
			continue
		}

		invalid++
	}

	fmt.Println(valid, invalid)
}