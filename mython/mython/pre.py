import html
import sys


def main():
    "read stdin, and convert to escaped preformatted"
    inp = sys.stdin.read()
    print("-" * 80)
    print("<pre>")
    print(html.escape(inp))
    print("</pre>")
 
if __name__ == "__main__" :
    main()
