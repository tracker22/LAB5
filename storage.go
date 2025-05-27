package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

// Константы склада
const (
	Zones      = "ABC"
	Shelves    = 20
	Vertical   = 5
	Horizontal = 2
	CellCap    = 10
	AddressLen = 5
)

var storage = make(map[string]map[string]int)

// Проверка корректности адреса
func isValidAddress(address string) bool {
	if len(address) != AddressLen {
		return false
	}

	if !strings.Contains(Zones, string(address[0])) {
		return false
	}

	shelf, err := strconv.Atoi(address[1:3])
	if err != nil || shelf < 1 || shelf > Shelves {
		return false
	}

	section, err := strconv.Atoi(string(address[3]))
	if err != nil || section < 1 || section > Vertical {
		return false
	}

	shelfNum, err := strconv.Atoi(string(address[4]))
	if err != nil || shelfNum < 1 || shelfNum > Horizontal {
		return false
	}

	return true
}

func itemsInCell(address string) int {
	total := 0
	for _, qty := range storage[address] {
		total += qty
	}
	return total
}

func addItem(name string, qty int, address string) {
	if !isValidAddress(address) {
		fmt.Printf("Ошибка: некорректный адрес %s\n", address)
		return
	}

	if itemsInCell(address)+qty > CellCap {
		fmt.Printf("Ошибка: в ячейке %s недостаточно места для %d единиц товара\n", address, qty)
		return
	}

	if storage[address] == nil {
		storage[address] = make(map[string]int)
	}
	storage[address][name] += qty
	fmt.Printf("Добавлено %d единиц товара '%s' в ячейку %s\n", qty, name, address)
}

func removeItem(name string, qty int, address string) {
	if !isValidAddress(address) {
		fmt.Printf("Ошибка: некорректный адрес %s\n", address)
		return
	}

	if storage[address][name] < qty {
		fmt.Printf("Ошибка: в ячейке %s недостаточно товара '%s' для списания\n", address, name)
		return
	}

	storage[address][name] -= qty
	if storage[address][name] == 0 {
		delete(storage[address], name)
	}
	fmt.Printf("Списано %d единиц товара '%s' из ячейки %s\n", qty, name, address)
}

func showInfo() {
	totalCells := len(Zones) * Shelves * Vertical * Horizontal
	usedCells := 0
	totalItems := 0

	for _, items := range storage {
		cellItems := 0
		for _, qty := range items {
			cellItems += qty
		}
		if cellItems > 0 {
			usedCells++
			totalItems += cellItems
		}
	}

	// Общая загруженность
	totalCap := totalCells * CellCap
	loadPercent := float64(totalItems) / float64(totalCap) * 100
	fmt.Printf("\nОбщая загруженность склада: %.1f%%\n", loadPercent)

	// Загруженность по зонам
	for _, zone := range Zones {
		zoneItems := 0
		for addr, items := range storage {
			if addr[0] == byte(zone) {
				for _, qty := range items {
					zoneItems += qty
				}
			}
		}
		zoneCap := Shelves * Vertical * Horizontal * CellCap
		zonePercent := float64(zoneItems) / float64(zoneCap) * 100
		fmt.Printf("Зона %c загружена на %.1f%%\n", zone, zonePercent)
	}

	// Содержимое ячеек
	fmt.Println("\nСодержимое ячеек:")
	for addr, items := range storage {
		if len(items) > 0 {
			fmt.Printf("Ячейка %s: ", addr)
			for name, qty := range items {
				fmt.Printf("%s - %d шт.; ", name, qty)
			}
			fmt.Println()
		}
	}

	// Пустые ячейки
	fmt.Println("\nПустые ячеки:")
	emptyCount := 0
	for _, zone := range Zones {
		for shelf := 1; shelf <= Shelves; shelf++ {
			for section := 1; section <= Vertical; section++ {
				for shelfNum := 1; shelfNum <= Horizontal; shelfNum++ {
					shelfStr := fmt.Sprintf("%02d", shelf)
					addr := string(zone) + shelfStr + strconv.Itoa(section) + strconv.Itoa(shelfNum)
					if len(storage[addr]) == 0 {
						if emptyCount > 0 && emptyCount%10 == 0 {
							fmt.Println()
						}
						fmt.Printf("%s ", addr)
						emptyCount++
					}
				}
			}
		}
	}
	fmt.Println()
}

func initStorage() {
	for _, zone := range Zones {
		for shelf := 1; shelf <= Shelves; shelf++ {
			for section := 1; section <= Vertical; section++ {
				for shelfNum := 1; shelfNum <= Horizontal; shelfNum++ {
					shelfStr := fmt.Sprintf("%02d", shelf)
					addr := string(zone) + shelfStr + strconv.Itoa(section) + strconv.Itoa(shelfNum)
					storage[addr] = make(map[string]int)
				}
			}
		}
	}
}

func main() {
	initStorage()
	fmt.Println("Программа учета товаров на складе")
	fmt.Println("Доступные команды:")
	fmt.Println("ADD <наименование> <количество> <адрес> - добавить товар")
	fmt.Println("REMOVE <наименование> <количество> <адрес> - удалить товар")
	fmt.Println("INFO - показать информацию о складе")
	fmt.Println("EXIT - выход из программы")

	scanner := bufio.NewScanner(os.Stdin)
	for {
		fmt.Print("\n> ")
		if !scanner.Scan() {
			break
		}

		input := strings.TrimSpace(scanner.Text())
		if input == "" {
			continue
		}

		switch input {
		case "INFO":
			showInfo()
		case "EXIT":
			return
		default:
			parts := strings.Fields(input)
			if len(parts) == 4 && (parts[0] == "ADD" || parts[0] == "REMOVE") {
				name := parts[1]
				qty, err := strconv.Atoi(parts[2])
				if err != nil {
					fmt.Println("Ошибка: некорректное количество товара")
					continue
				}
				addr := parts[3]

				if parts[0] == "ADD" {
					addItem(name, qty, addr)
				} else {
					removeItem(name, qty, addr)
				}
			} else {
				fmt.Println("Ошибка: неизвестная команда или неверный формат")
			}
		}
	}
}
