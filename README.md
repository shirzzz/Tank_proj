# Tank_proj
דברים שצריך לעשות:
Game steps מערך של צעדים לכל שחקן:
2. להבין את איך לעשות את הgamesteps
3. לסדר את הצעד אחורה
4. אחרי שהוא צעד אחורה כל צעד אחורה מיידית אחרי זה יהיה בעלות של צעד אחד,
5. A backward move action that was not performed yet,
can be canceled by a forward move action request, in which case the tank will stay(!) in place
6. אולי להוסיף שדה שבודק מתי הפעם האחרונה שהייתה צעד אחורה ולנהל את הצעדים לפי זה
7. after shooting, the tank cannot shoot again for 4 game steps.

8. לסדר יציאה מהמגרש של הshell
9. רוצים שהshell יעשה פעמיים צעד אחד
10. אם נגמרו לשניהם הכדורים המשחק אמור להגמר אחרי 40 צעדים 
11. אפשר להוסיף לכל טנק שדה של כמה כדורים יש לו
12. צריך להגיד שמישהו ניצח או שזה תיקו
13. אם משהו יוצא מהמגרש הוא נכנס בחזרה מהצד השני  -
לשנות את הפונקציות של תזוזה קדימה ואחורה שיתייחסו לזה ( להוסיף if)
14. אם הכדורים מתנגשים שניהם מתפוצצים
15. להוסיף שאם טנק דורך על מוכש שני הטנק והמוכש מתפוצצים
16. אם 2 הטנקים מתנגשים שניהם מתפוצצים

17.אפשר למשל להריץ את לולאת המשחק בקצב של הקליע ורק בסיבובים זוגיים לבצע תנועות של הטנקים.
או כל פתרון אחר שתבחרו
18. ההצעה שלי היא להניע את כל הקליעים קודם.
ואז לבדוק אם יש פגיעה בטנקים (או עבור כל קליע בנפרד). 
ואז להזיז את הטנקים. ואז לבדוק שוב אם טנק נע לתוך קליע (או מוקש). 
כלומר - לבצע את הבדיקה גם לאחר תנועת קליע וגם לאחר תנועת טנק ולהתחיל קודם בתנועת הקליע.
19. אם הטנקים/הכדורים חולפים זה על פני זה - מחליפים מקומות זה אומר שהם התנגשו
20. לעשות שבלוח יהיה vector של מי שנמצא באותה משבצת
21. The “GameManager” should also handle cases in which the algorithm requests an illegal move
(e.g. stepping into a wall, or requesting an invalid move such as reshooting without waiting 4 game steps), 
it should ignore it but write it to the output file as a “bad step”.
22. At least one of the two different algorithms that you are required to submit should 
try to chase the opponent’s tank 
(using DFS or BFS algorithm, try not to calculate the entire path for each step!). 
You can decide that the other algorithm is only triggered for moving a tank if being chased by a shell.
23. צריך שהצעדים ישמרו לכל טנק את הצעדים שלו ולא לשניהם ביחד
24. מחלקה לכל הshells

in the end:
Output File - TEXT file
The output file should include:
all the steps performed by each player, including “bad steps”
the result - win (who) or tie, and the reason

Error Handling
You should reasonably handle any error:
Program shall not crash, never.
You should better recover from input file errors (e.g. if a player has more than 1 tank, take the 1st, ignore the rest; if dimensions of the board do not fit the declared dimensions in file: ignore rows and columns beyond the declared dimensions,
fill in spaces for missing chars and lines; treat unspecified chars as space). You should write a short description of all recovered errors found in the input file, into input_errors.txt file. Create this file only if there are errors.
In case there is an unrecoverable error, a message should be printed to screen before the program finishes. No need for input_errors.txt file in this case.
You should not end the program using the “exit” function or similar functions, even in error cases. Program shall end by finishing main, in all scenarios.
