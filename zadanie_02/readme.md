/*

Wykonał: Jakub Szubartowski (A01/C01/L02)   I Informatyka        | Planowany termin:
                email: 31024@s.pm.szczecin.pl                    | approx: 3 tyg
                 ZADANIE 2: "PROBLEM HETMANOW"                   | (04.06.2024)

*/

		               PLAN DZIAŁANIA
	      (nazwa projektu: "Problem Hetmanow")

1. Definicja problemu: Problem hetmanów polega na umieszczeniu N hetmanów na szachownicy NxN tak, aby żaden z nich nie atakował innego.

2. Inicjalizacja planszy: Stworzyć planszę NxN za pomocą dwuwymiarowej tablicy.
   Wszystkie pola powinny być początkowo ustawione na 0, co oznacza, że nie ma na nich hetmana.

3. Rekurencyjne rozwiązanie: Napisać funkcję rekurencyjną, która umieszcza hetmany na planszy.
   Zaczyna od lewej kolumny i przechodzi do prawej, próbując umieścić hetmana w każdym rzędzie.

4. Sprawdzanie bezpieczeństwa: W funkcji rekurencyjnej, przed umieszczeniem hetmana na danym polu, należy sprawdzić, czy jest bezpieczne.
   To znaczy, czy żaden inny hetman nie atakuje tego pola. Można to zrobić sprawdzając, czy w tym samym rzędzie, kolumnie lub na przekątnych jest już hetman.

5. Backtracking: Jeśli umieszczenie hetmana na danym polu prowadzi do rozwiązania, kontynuuj rekurencję.
   Jeśli nie, usuń hetmana z tego pola (ustaw wartość na 0) i kontynuuj próby w następnych rzędach.

6. Znalezienie wszystkich rozwiązań: Jeśli chce się znaleźć wszystkie możliwe rozwiązania, nie zatrzymuj się po znalezieniu pierwszego rozwiązania.
   Zamiast tego kontynuuj rekurencję i szukaj dalszych rozwiązań.

7. Wyświetlanie wyników: Po znalezieniu rozwiązania (lub wszystkich rozwiązań), wyświetla planszę na ekranie.
   Hetmany mogą być reprezentowane przez 1, a puste pola przez 0.
