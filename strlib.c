/*
 * ============================================================================
 *  Assignment: Your Own String Library
 *  Course:     Programming with C
 *
 *  Name:       Munyaneza Robben
 *  Student ID: 20251SEN284
 * ============================================================================
 *
 *  HOW TO COMPILE AND RUN
 *      gcc -Wall -Wextra -g strlib.c -o strlib
 *      ./strlib
 *
 *  Fix EVERY warning: the autograder treats any warning as a compile failure.
 *
 *  QUICK RULES (see the README for details)
 *   - No library functions inside the seven functions: no strlen, strcpy,
 *     strcmp, tolower, isspace, printf, malloc, ...
 *   - Do not change any function's name, return type, or parameters.
 *   - Helper functions are encouraged. Give them your own names (my_strlen,
 *     not strlen), and use every helper you write or delete it: an unused
 *     function or variable is a warning, and a warning is a failure.
 *   - Every string is plain ASCII. Pointer arguments are never NULL.
 *
 *  ABOUT THE "YOUR NOTES" BOXES
 *   Each function has a box for you to fill in: your approach, the edge cases
 *   you handled, and how you tested it. The notes are not graded, but writing
 *   them is the best way to check that you understand your own code, and you
 *   may be asked to explain these functions in class or on the exam.
 * ============================================================================
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdio.h>    /* printf: ONLY for testing in main() */


/* ============================================================================
 *  HELPER FUNCTIONS (optional, write your own here)
 * ============================================================================
 *
 *  Ideas that several functions below can share:
 *
 *   - size_t my_strlen(const char *s)
 *       Walk a pointer to the '\0' and return how far it went.
 *       Useful in: str_reverse, str_copy_bounded.
 *
 *   - char my_to_lower(char c)
 *       Turn 'A'..'Z' into 'a'..'z'; return any other character unchanged.
 *       Hint: in ASCII, 'a' - 'A' is the same for every letter.
 *       Useful in: str_compare_ci.
 *
 *   - int is_blank(char c)
 *       1 if c is ' ' or '\t', otherwise 0. Nothing else counts!
 *       Useful in: str_squeeze.
 *
 *   - int matches_at(const char *p, const char *needle)
 *       1 if needle appears starting exactly at p, otherwise 0.
 *       Useful in: str_find_sub. With this helper, the main function becomes
 *       "try every starting position", which is much easier to get right.
 *
 *  Write each helper ABOVE the functions that use it.
 */

/* Length of s: walk p to the '\0' and return how far it moved. */
static size_t my_strlen(const char *s)
{
    const char *p = s;
    while (*p != '\0')
        p++;
    return (size_t)(p - s);
}

/* 'A'..'Z' become 'a'..'z'; every other character is returned unchanged.
 * In ASCII the gap 'a' - 'A' is 32 for every letter. */
static char my_to_lower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return (char)(c + ('a' - 'A'));
    return c;
}

/* 1 for space or tab, 0 for anything else (including '\n'). */
static int is_blank(char c)
{
    return c == ' ' || c == '\t';
}

/* 1 if the whole needle appears starting exactly at p, otherwise 0.
 * If p hits its '\0' before needle ends, then *p ('\0') differs from
 * *needle (not '\0'), so we return 0 and never read past p's end. */
static int matches_at(const char *p, const char *needle)
{
    while (*needle != '\0') {
        if (*p != *needle)
            return 0;
        p++;
        needle++;
    }
    return 1;
}



/* ============================================================================
 *  1. str_find_char
 * ============================================================================
 *  Returns a pointer to the FIRST occurrence of c in s, or NULL if c does not
 *  appear. Searching for '\0' returns a pointer to s's terminating '\0'.
 *
 *  HINTS
 *   - Walk a pointer through s. At each position, check for c BEFORE you
 *     check for the end of the string. Why does the order matter when
 *     c is '\0'?
 *   - You are returning a position inside s, not a copy of anything.
 *
 *  YOUR NOTES
 *   Approach: walk p through s. At each position first ask "is this c?"
 *     and only then "is this the end?". Return p itself (a position inside
 *     s), not a copy.
 *
 *   Edge cases I handled: c == '\0' -> the c-check fires on the terminator,
 *     so we return s + length. Empty s -> only the '\0' is checked. c not
 *     present -> we reach '\0' and return NULL. Case matters ('H' != 'h').
 *
 *   How I tested it: visible tests + main(): "hello" with 'l','h','z','H',
 *     '\0', and "" with 'a' and '\0'.
 *
 */
const char *str_find_char(const char *s, char c)
{
    const char *p = s;

    for (;;) {
        if (*p == c)        /* check c FIRST, so searching for '\0' works */
            return p;
        if (*p == '\0')     /* reached the end without finding c */
            return NULL;
        p++;
    }
}


/* ============================================================================
 *  2. str_reverse
 * ============================================================================
 *  Reverses s in place. Do not use a second array.
 *
 *  HINTS
 *   - Use one pointer at the first character and one at the LAST character
 *     (not at the '\0'). Swap what they point to, then move them toward
 *     each other.
 *   - When should the loop stop? Check by hand for "abc" and "abcd".
 *   - Careful with "": there is no last character. Make sure your end pointer
 *     never goes before the start of the string.
 *
 *  YOUR NOTES
 *   Approach: left starts at the first character, right at the LAST
 *     character (s + len - 1, not the '\0'). Swap them, move them toward
 *     each other, stop when they meet or cross (left >= right).
 *
 *   Edge cases I handled: "" and one-character strings return before
 *     computing s + len - 1, so right never points before s. Odd length
 *     ("abc"): pointers meet on the middle char, which stays put. Even
 *     length ("abcd"): pointers cross after the last swap.
 *
 *   How I tested it: "hello" -> "olleh", "ab" -> "ba", "a", "", "racecar".
 *
 */
void str_reverse(char *s)
{
    size_t len = my_strlen(s);

    if (len < 2)            /* "" or one char: nothing to swap */
        return;

    char *left = s;
    char *right = s + len - 1;

    while (left < right) {
        char tmp = *left;
        *left = *right;
        *right = tmp;
        left++;
        right--;
    }
}


/* ============================================================================
 *  3. str_compare_ci
 * ============================================================================
 *  Compares a and b ignoring case. Returns EXACTLY -1, 0, or 1.
 *  Convert uppercase letters to LOWERCASE before comparing (not uppercase).
 *  If one string ends first, the shorter one comes first.
 *
 *  HINTS
 *   - Walk both strings together, one character at a time.
 *   - Stop at the first position where the (lowercased) characters differ,
 *     or when you reach the end of either string.
 *   - When the loop ends, there are three possibilities: both strings ended,
 *     only a ended, or only b ended. Which result does each give?
 *   - Return -1 or 1 yourself. Do not return the difference of two
 *     characters: 'a' - 'z' is -25, not -1.
 *
 *  YOUR NOTES
 *   Approach: walk pa and pb together. Lowercase both characters (only
 *     'A'..'Z' change) and compare; the first difference decides -1 or 1.
 *     If the loop ends because a string ran out, decide by who ended.
 *
 *   Edge cases I handled: both empty -> 0. Prefix ("ab" vs "abc") -> the
 *     shorter one is -1. Digits/symbols are not changed. Lowercase (not
 *     uppercase) matters for "a_b" vs "aBb": '_' (95) < 'b' (98) -> -1.
 *     Always returns exactly -1, 0 or 1, never a raw difference.
 *
 *   How I tested it: "Hello"/"hello", "apple"/"Banana", "Zebra"/"apple",
 *     "abc"/"ab", "AB"/"abc", ""/"", ""/"a", "123"/"123", "a_b"/"aBb".
 *
 */
int str_compare_ci(const char *a, const char *b)
{
    const char *pa = a;
    const char *pb = b;

    while (*pa != '\0' && *pb != '\0') {
        char ca = my_to_lower(*pa);
        char cb = my_to_lower(*pb);
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        pa++;
        pb++;
    }

    /* At least one string ended. */
    if (*pa == '\0' && *pb == '\0')
        return 0;           /* both ended together: equal */
    if (*pa == '\0')
        return -1;          /* a ended first: a is shorter, comes first */
    return 1;               /* b ended first */
}


/* ============================================================================
 *  4. str_copy_bounded
 * ============================================================================
 *  Copies src into dst, where size is the TOTAL number of bytes in dst.
 *   - Copy at most size - 1 characters, then write '\0'.
 *   - If size is 0, write nothing at all.
 *   - Never write past dst[size - 1], and write nothing after the '\0'.
 *   - Return the length of src (NOT the number of characters copied).
 *     The caller detects truncation with: return value >= size.
 *
 *  HINTS
 *   - Your copy loop needs TWO reasons to stop: the end of src, and running
 *     out of room in dst. Which one comes first depends on the input.
 *   - If you move src forward while copying, it no longer points to the
 *     start of the string. Keep a pointer to the start if you need it later.
 *   - size is a size_t, which is unsigned: it can never be negative.
 *     What is size - 1 when size is 0? Handle size 0 before doing any math.
 *   - Draw dst as a row of boxes and fill them in by hand for
 *     ("hello", size 4) before you write any code.
 *
 *  YOUR NOTES
 *   Approach: measure src first (that is the return value). If size is 0,
 *     stop. Otherwise copy while there is room (i < size - 1) AND src has
 *     not ended, then write one '\0' at dst[i].
 *
 *   Edge cases I handled: size 0 -> nothing written, and we never compute
 *     size - 1 (which would wrap to a huge number because size_t is
 *     unsigned). size 1 -> only '\0' written. Exact fit ("abc", 4) and
 *     off-by-one truncation ("abc", 3). Empty src -> dst is "". The highest
 *     index ever written is i <= size - 1, and nothing after the '\0'.
 *
 *   How I tested it: ("hello",10), ("hello",4) -> "hel" returns 5,
 *     ("abc",4), ("abc",3), ("hello",1), ("hello",0), ("",10).
 *
 */
size_t str_copy_bounded(char *dst, const char *src, size_t size)
{
    size_t src_len = my_strlen(src);

    if (size == 0)          /* no room at all: write nothing */
        return src_len;

    size_t i = 0;
    while (i < size - 1 && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';          /* i <= size - 1, so this stays inside dst */

    return src_len;         /* caller detects truncation: src_len >= size */
}


/* ============================================================================
 *  5. str_find_sub
 * ============================================================================
 *  Returns a pointer to the FIRST place in hay where all of needle appears,
 *  or NULL if it never does. An empty needle is found at the start of hay.
 *
 *  HINTS
 *   - Try every starting position in hay, one at a time. At each one, check
 *     whether the whole needle matches from there (the matches_at helper).
 *   - If a match fails halfway, the next attempt starts ONE position after
 *     the previous START, not where the failure happened. Trace "aaab" and
 *     "aab" by hand to see why.
 *   - Never read past hay's '\0'. What should happen if hay ends while you
 *     are still in the middle of matching needle?
 *   - Pointers here are const char *. Any pointer you create to walk through
 *     hay or needle must be const char * as well, or you'll get a warning.
 *
 *  YOUR NOTES
 *   Approach: try every starting position in hay, one at a time. At each
 *     one, matches_at() checks whether the whole needle matches there. On
 *     a failed attempt we move start forward by ONE, never jump ahead.
 *
 *   Edge cases I handled: empty needle -> matches_at returns 1 at once, so
 *     we return hay (also for hay ""). Needle longer than hay -> matches_at
 *     fails when hay hits '\0'; we stop after trying the '\0' position, so
 *     we never read past hay's end. Overlap: "aaab"/"aab" fails at index 0
 *     and succeeds at index 1. Case matters.
 *
 *   How I tested it: "hello world" with "world","o","hello world","World";
 *     "abc"/"abcd"; "mississippi"/"issip" -> 4; "aaab"/"aab" -> 1;
 *     "abc"/""; ""/""; ""/"a".
 *
 */
const char *str_find_sub(const char *hay, const char *needle)
{
    const char *start = hay;

    for (;;) {
        if (matches_at(start, needle))
            return start;
        if (*start == '\0')     /* tried every position, including the end */
            return NULL;
        start++;
    }
}


/* ============================================================================
 *  6. str_squeeze
 * ============================================================================
 *  Cleans up whitespace in s, in place, and returns the new length.
 *  Whitespace means ONLY ' ' and '\t' ('\n' is an ordinary character here).
 *   - Each run of whitespace between other characters becomes ONE space.
 *   - Whitespace at the start and end is removed completely.
 *
 *  HINTS
 *   - Use two pointers into the same array: a READ pointer that visits every
 *     character once, and a WRITE pointer that marks where the next kept
 *     character goes. Write never gets ahead of read. Why not?
 *   - When the read pointer reaches whitespace, skip the WHOLE run first.
 *     Then look at what comes next: more text, or the '\0'?
 *   - Always finish by writing '\0' at the write pointer.
 *   - The new length is how far the write pointer moved.
 *
 *  YOUR NOTES
 *   Approach: two pointers into the same array. read visits every char,
 *     write marks where the next kept char goes. Skip leading blanks. Then
 *     copy ordinary chars; when we hit a run of blanks, skip the whole run
 *     and write ONE ' ' only if more text follows (not if we hit '\0').
 *     Finish with '\0' at write. New length = write - s.
 *
 *   Why write never passes read: every kept char moves both by one, and a
 *     blank run of length >= 1 is replaced by at most one ' ', so write
 *     only ever falls behind.
 *
 *   Edge cases I handled: only blanks ("   ", " \t ") -> "". Empty "" ->
 *     "". Trailing blanks dropped. Tabs count as blanks, '\n' does NOT
 *     ("a  \n  b" -> "a \n b").
 *
 *   How I tested it: "  hello   world  " -> 11, "a\t\tb" -> 3, "one two",
 *     "   ", "", " \t a \t ", "a  \n  b" -> 5.
 *
 */
size_t str_squeeze(char *s)
{
    char *read = s;
    char *write = s;

    while (is_blank(*read))         /* drop leading whitespace */
        read++;

    while (*read != '\0') {
        if (is_blank(*read)) {
            while (is_blank(*read)) /* skip the whole run */
                read++;
            if (*read != '\0') {    /* text follows: keep one space */
                *write = ' ';
                write++;
            }                       /* else: trailing run, drop it */
        } else {
            *write = *read;
            write++;
            read++;
        }
    }
    *write = '\0';

    return (size_t)(write - s);
}


/* ============================================================================
 *  7. str_split
 * ============================================================================
 *  Splits s into fields separated by delim, in place.
 *   - Replace EVERY delim in s with '\0'.
 *   - Store a pointer to the start of each field in fields[0], fields[1], ...
 *     but store at most max_fields pointers. Leave the rest of fields alone.
 *   - Return the TOTAL number of fields, even if it is more than max_fields.
 *   - A string with k delimiters has exactly k + 1 fields. Empty fields count.
 *
 *  HINTS
 *   - Keep a pointer to where the current field starts. Each time you find a
 *     delimiter, the current field ends there and a new one starts right
 *     after it.
 *   - The last field is never followed by a delimiter. Where does it start,
 *     and when do you store it?
 *   - Before EVERY write to fields[i], check that i < max_fields. Inside this
 *     function you cannot know how big the caller's array is.
 *   - If your logic is right, the empty string "" needs no special case.
 *     It has zero delimiters, so how many fields?
 *
 *  YOUR NOTES
 *   Approach: field_start remembers where the current field begins. Walk p
 *     through s. At each delim: record field_start (if there is room),
 *     count it, replace the delim with '\0', and the next field starts at
 *     p + 1. After the loop, the last field (never followed by a delim) is
 *     recorded and counted the same way.
 *
 *   Edge cases I handled: k delimiters always give k + 1 fields. Empty
 *     fields ("a,,b", ",a,") are counted and stored. "" -> 1 field "".
 *     More fields than max_fields -> every delim is still replaced, only
 *     the first max_fields pointers are stored, and the full count is
 *     returned. max_fields 0 -> fields is never touched. Every write to
 *     fields[] is guarded by count < max_fields.
 *
 *   How I tested it: "red,green,blue", "a,,b", ",a,", "w,x,y,z" with max 2,
 *     "no delimiters", "", "a,b" with max 0.
 *
 */
int str_split(char *s, char delim, char *fields[], int max_fields)
{
    int count = 0;
    char *field_start = s;

    for (char *p = s; *p != '\0'; p++) {
        if (*p == delim) {
            if (count < max_fields)
                fields[count] = field_start;
            count++;
            *p = '\0';              /* cut the string here */
            field_start = p + 1;    /* next field begins after the cut */
        }
    }

    /* The last field ends at the original '\0'. */
    if (count < max_fields)
        fields[count] = field_start;
    count++;

    return count;
}


/* ============================================================================
 *  main: YOUR testing area. The autograder ignores everything in here.
 * ============================================================================
 *  A few sample calls to get you started. Add your own, especially for the
 *  edge cases in the README. Remember: functions that MODIFY a string need a
 *  char array (char s[] = "..."), not a string literal (char *s = "...").
 */
int main(void)
{
    /* 1. str_find_char: print the offset, or NULL */
    const char *word = "hello";
    const char *pos = str_find_char(word, 'l');
    if (pos != NULL)
        printf("find_char: offset %d (expected 2)\n", (int)(pos - word));
    else
        printf("find_char: NULL (expected offset 2)\n");

    /* 2. str_reverse: needs an array */
    char rev[] = "hello";
    str_reverse(rev);
    printf("reverse: \"%s\" (expected \"olleh\")\n", rev);

    /* 3. str_compare_ci */
    printf("compare_ci: %d (expected -1)\n", str_compare_ci("apple", "Banana"));

    /* 4. str_copy_bounded: truncation */
    char small[4] = "";   /* starts empty, so printing is safe even before you write the function */
    size_t n = str_copy_bounded(small, "hello", sizeof small);
    printf("copy_bounded: returned %d, dst \"%s\" (expected 5, \"hel\")\n",
           (int)n, small);

    /* 5. str_find_sub */
    const char *hay = "aaab";
    const char *found = str_find_sub(hay, "aab");
    if (found != NULL)
        printf("find_sub: offset %d (expected 1)\n", (int)(found - hay));
    else
        printf("find_sub: NULL (expected offset 1)\n");

    /* 6. str_squeeze: brackets make leftover spaces visible */
    char messy[] = "  hello   world  ";
    size_t len = str_squeeze(messy);
    printf("squeeze: returned %d, [%s] (expected 11, [hello world])\n",
           (int)len, messy);

    /* 7. str_split */
    char line[] = "red,green,blue";
    char *fields[5];
    int count = str_split(line, ',', fields, 5);
    printf("split: returned %d (expected 3)\n", count);
    for (int i = 0; i < count && i < 5; i++)
        printf("  fields[%d] = [%s]\n", i, fields[i]);

    /* ---- extra edge cases from the README ---- */
    printf("\n--- edge cases ---\n");

    printf("find_char '\\0' in \"hello\": offset %d (expected 5)\n",
           (int)(str_find_char(word, '\0') - word));
    printf("find_char 'H' in \"hello\": %s (expected NULL)\n",
           str_find_char(word, 'H') == NULL ? "NULL" : "found");
    printf("find_char 'a' in \"\": %s (expected NULL)\n",
           str_find_char("", 'a') == NULL ? "NULL" : "found");

    char r1[] = "";   str_reverse(r1);
    char r2[] = "a";  str_reverse(r2);
    char r3[] = "ab"; str_reverse(r3);
    printf("reverse: [%s] [%s] [%s] (expected [] [a] [ba])\n", r1, r2, r3);

    printf("compare_ci: %d %d %d %d (expected 0 1 -1 -1)\n",
           str_compare_ci("", ""), str_compare_ci("abc", "ab"),
           str_compare_ci("", "a"), str_compare_ci("a_b", "aBb"));

    char d10[10] = "xxxxxxxxx";
    n = str_copy_bounded(d10, "abc", 3);
    printf("copy_bounded(\"abc\", 3): returned %d, dst \"%s\" (expected 3, \"ab\")\n",
           (int)n, d10);
    n = str_copy_bounded(d10, "hello", 0);
    printf("copy_bounded(size 0): returned %d, dst \"%s\" (expected 5, \"ab\" untouched)\n",
           (int)n, d10);

    const char *m = "mississippi";
    printf("find_sub: %d (expected 4), empty needle %s, \"\" in \"\" %s, \"a\" in \"\" %s\n",
           (int)(str_find_sub(m, "issip") - m),
           str_find_sub("abc", "") != NULL ? "found" : "NULL",
           str_find_sub("", "") != NULL ? "found" : "NULL",
           str_find_sub("", "a") == NULL ? "NULL" : "found");

    char q1[] = "   ";
    char q2[] = " \t a \t ";
    char q3[] = "a  \n  b";
    size_t l1 = str_squeeze(q1), l2 = str_squeeze(q2), l3 = str_squeeze(q3);
    printf("squeeze: %d [%s], %d [%s], %d (expected 0 [], 1 [a], 5)\n",
           (int)l1, q1, (int)l2, q2, (int)l3);

    char s1[] = ",a,";
    char s2[] = "";
    char s3[] = "w,x,y,z";
    char *f2[2];
    printf("split: %d %d %d (expected 3 1 4)\n",
           str_split(s1, ',', fields, 5), str_split(s2, ',', fields, 5),
           str_split(s3, ',', f2, 2));

    return 0;
}
