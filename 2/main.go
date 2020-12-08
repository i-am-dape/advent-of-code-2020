package main

import "bufio"
import "fmt"
import "os"
import "strconv"
import "strings"

func parse(txt string) (int, int, byte, string, error) {
	dashAt := strings.Index(txt, "-")
	minTxt := txt[:dashAt]
	txt = txt[dashAt+1:]
	min, err := strconv.Atoi(minTxt)

	spaceAt := strings.Index(txt, " ")
	maxTxt := txt[:spaceAt]
	txt = txt[spaceAt+1:]
	max, err := strconv.Atoi(maxTxt)

	colonAt := spaceAt + 2
	charTxt := txt[:colonAt]	
	txt = txt[colonAt-1:]

	char := byte(charTxt[0])
	txt = strings.TrimSpace(txt)

	return min, max, char, txt, err
}

func validate(trace bool, txt string) bool {
	// format
	// <min>-<max> <char>: <password>
	min, max, char, pwd, err := parse(txt)
	if trace {
		fmt.Println()
		fmt.Println("raw:", txt)
		fmt.Printf("min:%d max:%d char:%d %v %q %v", min, max, char, string(char), pwd, err)
		fmt.Println()
	}
	if err != nil {
		return false
	}

	if char < 'a' || char > 'z' {
		fmt.Println(char, string(char))
		panic("err")
	}

	revTxt := fmt.Sprintf("%d-%d %s: %s", min, max, string(char), pwd)
	if revTxt != txt {
		fmt.Println("in:", txt, []byte(txt))
		fmt.Println("re:", revTxt, []byte(revTxt))
	}

	//fmt.Println(min, max, char, pwd, err)
	matches := strings.Count(pwd, string(char))
	if trace {
		fmt.Println("matches", matches, string(char), pwd)
	}
	return matches >= min && matches <= max
}

func processStdin() (int, int, int) {
	scanner := bufio.NewScanner(os.Stdin)
	ok := 0
	bad := 0
	row := 0
	for scanner.Scan() {
		row++
		if validate(false, scanner.Text()) {
			ok++
		} else {
			bad++
		}
	}
	return ok, bad, ok + bad
}


func main() {
	//fmt.Println(validate(true, "1-10 c: wcmvcrcmjmcmbcckfcdc"))
	fmt.Println(validate(true, "1-3 a: abcde"))
	fmt.Println(validate(true, "1-3 b: cdefg"))
	fmt.Println(validate(true, "1-3 b:  cdefg"))
	fmt.Println(validate(true, "2-9 c: ccccccccc"))
	fmt.Println(validate(true, "2-9 c:   ccccccccc"))
	fmt.Println(validate(true, "2-9 c: ccccccccc "))

	fmt.Println(processStdin())
}
