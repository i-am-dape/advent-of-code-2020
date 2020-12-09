package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type passport map[string]string

type validFunc func(string) bool

func inRange(min, v, max int) bool {
	return v >= min && v <= max
}

func makeMinMax(min, max int) func(string) bool {
	return func(txt string) bool {
		v, err := strconv.Atoi(txt)
		if err != nil {
			return false
		}

		return inRange(min, v, max)
	}
}

func validateHeight(txt string) bool {
	var min, max int
	if strings.HasSuffix(txt, "in") {
		min = 59
		max = 76

	} else if strings.HasSuffix(txt, "cm") {
		min = 150
		max = 193
	} else {
		return false
	}

	v, err := strconv.Atoi(txt[:len(txt)-2])
	if err != nil {
		return false
	}

	return inRange(min, v, max)
}

func validateHair(txt string) bool {
	if txt[0] != '#' {
		fmt.Println("missing prefix", txt)
		return false
	}

	for _, c := range txt[1:] {
		if c >= '0' && c <= '9' {
			continue
		}

		if c >= 'a' && c <= 'f' {
			continue
		}

		fmt.Println("bad char:", c)
		return false
	}

	return true
}

var colors = map[string]interface{}{
	"amb": 0,
	"blu": 0,
	"brn": 0,
	"gry": 0,
	"grn": 0,
	"hzl": 0,
	"oth": 0,
}

func validateEye(txt string) bool {
	_, ok := colors[txt]
	return ok
}

func validatePid(txt string) bool {
	if len(txt) != 9 {
		fmt.Println("pid to short", txt, len(txt))
		return false
	}

	for _, c := range txt {
		if c >= '0' && c <= '9' {
			continue
		}

		return false
	}
	return true
}

func ignored(string) bool {
	return true
}

type field struct {
	req     bool
	isValid validFunc
}

var fields = map[string]field{
	"byr": {true, makeMinMax(1920, 2002)},
	"iyr": {true, makeMinMax(2010, 2020)},
	"eyr": {true, makeMinMax(2020, 2030)},
	"hgt": {true, validateHeight},
	"hcl": {true, validateHair},
	"ecl": {true, validateEye},
	"pid": {true, validatePid},
	"cid": {false, ignored},
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

func validate(p passport) bool {
	for field, desc := range fields {
		txt, ok := p[field]
		if !ok && !desc.req {
			// missing but not required
			continue
		}

		if !ok {
			// missing but required
			fmt.Println("missing", p, field)
			return false
		}

		if !desc.isValid(txt) {
			// invalid data
			fmt.Println("bad", p, field, txt)
			return false
		}
	}

	return true
}

func main() {
	fmt.Println("4")
	passports := processStdin()
	valid := 0
	invalid := 0
	for _, passport := range passports {
		cnt := &invalid
		if validate(passport) {
			cnt = &valid
		}

		(*cnt)++
	}

	fmt.Println(valid, invalid)
}
