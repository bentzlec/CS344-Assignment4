1. Compile using "gcc -std=gnu99 -pthread -o line_processor main.c"
2. Read the note below
3. Run using "./line_processor < input.txt > output.txt" or "./line_processor < input.txt" or "./line_processor" etc., depending on how you want to file redirect.
4. Delete the line_processor using "rm line_processor"
5. Repeat steps 1, 3, 4.

Important note: My function to replace "++" with '^' has weird behavior. 
I used adapted a function from the previous assignment, and it worked flawlessy for changing "$$" into the pid.
If you print buffer3 by uncommenting some of the code I left in main you will see something like this:

Input: "++rust++"   Output: "^rust(broken character here)"

Additionally, when I pull the string from the buffer it to print it, it reverts back to original "++rust++. But sometimes it also breaks,
which I think is as a result of the weird characters from my function. Whenever it broke I ran a different file with no "++"'s in it, and then ran
the file with "++"s in it again.
