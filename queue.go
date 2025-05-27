package main

import (
	"fmt"
	"math/rand"
	"strconv"
	"strings"
)

// Генерация номера талона (как в оригинале)
func generateTicket() string {
	return "T" + strconv.Itoa(rand.Int()%1000)
}

func main() {
	var windows int
	fmt.Print(">>> Введите кол-во окон\n<<< ")
	fmt.Scan(&windows)

	// Посетители: слайс пар (время, талон)
	visitors := make([][2]interface{}, 0)
	windowTimes := make([]int, windows)
	windowTickets := make([][]string, windows)

	var cmd string
	for {
		fmt.Print("<<< ")
		fmt.Scan(&cmd)

		switch cmd {
		case "ENQUEUE":
			var time int
			fmt.Scan(&time)
			ticket := generateTicket()
			visitors = append(visitors, [2]interface{}{time, ticket})
			fmt.Printf(">>> %s\n", ticket)

		case "DISTRIBUTE":
			// Распределение посетителей по окнам
			for _, visitor := range visitors {
				// Находим окно с минимальным временем
				minIndex := 0
				for i := 1; i < windows; i++ {
					if windowTimes[i] < windowTimes[minIndex] {
						minIndex = i
					}
				}

				// Добавляем посетителя в окно
				time := visitor[0].(int)
				ticket := visitor[1].(string)
				windowTimes[minIndex] += time
				windowTickets[minIndex] = append(windowTickets[minIndex], ticket)
			}

			// Вывод результатов
			for i := 0; i < windows; i++ {
				if len(windowTickets[i]) > 0 {
					fmt.Printf(">>> Окно %d (%d минут): %s\n",
						i+1,
						windowTimes[i],
						strings.Join(windowTickets[i], ", "))
				}
			}
			return

		default:
			fmt.Println(">>> Неизвестная команда")
		}
	}
}
