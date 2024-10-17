
def readFile(fp: str) -> str:
    with open(fp, 'r') as file:
        return file.read()

def countLines(fp: str) -> int:
    content = readFile(fp)
    return len(content.split('\n'))

def countExcludingBlankLines(fp: str) -> int:
    count = 0
    lines = readFile(fp).strip().split('\n')
    
    for line in lines:
        line = line.strip()
        if line not in ['', None, '\n', ' ', '\r']:
            count += 1
    
    return count
    

if __name__ == '__main__':
    fp = "Assignment_2203077_CSE1204.cpp"
    
    totalLines = countLines(fp)
    codeContainingLines = countExcludingBlankLines(fp)
    blankLines = totalLines - codeContainingLines
    
    print(f"Total Characters: {len(readFile(fp))}")
    print(f"{totalLines = }")
    print(f"{blankLines = }")
    print(f"{codeContainingLines = }")
