import re
import argparse
from pathlib import Path

def ConvertHeaderToASM(inputFile: Path, outputFile: Path):
    with inputFile.open('r') as file:
        lines = file.readlines()

    definedMacros = {}
    orderedEqus = []
    rawEnumLines = []
    insideEnum = False
    inBlockComment = False

    # Process #defines, preserve pure comments, and collect enums
    for raw in lines:
        raw = raw.rstrip('\n')
        stripped = raw.strip()

        # Preserve whole-line comments as ASM comments
        if stripped.startswith('//'):
            comment = stripped[2:].strip()
            orderedEqus.append(f"\n@ {comment}")
            continue
        if stripped.startswith('/*') and stripped.endswith('*/'):
            comment = stripped[2:-2].strip()
            orderedEqus.append(f"\n@ {comment}")
            continue

        line = stripped

        # Handle block comments
        if inBlockComment:
            if '*/' in line:
                inBlockComment = False
                line = line.split('*/', 1)[1].strip()
            else:
                continue

        if '/*' in line:
            if '*/' in line:
                line = re.sub(r'/\*.*?\*/', '', line)  # Remove inline block comment
            else:
                inBlockComment = True
                line = line.split('/*', 1)[0].strip()

        # Strip trailing single-line comments
        line = line.split('//', 1)[0].strip()
        if not line:
            continue

        # Simple #define (decimal or hex)
        defineMatch = re.match(r"#define\s+(\w+)\s+(0x[0-9A-Fa-f]+|\d+)", line)
        if defineMatch:
            name, val = defineMatch.groups()
            definedMacros[name] = int(val, 0)
            orderedEqus.append(f".equ {name}, {definedMacros[name]}")
            continue

        # Macro expression like BASE + 1
        exprMatch = re.match(r"#define\s+(\w+)\s+(.+)", line)
        if exprMatch:
            name, expr = exprMatch.groups()
            try:
                value = eval(expr, {"__builtins__": None}, definedMacros)
                definedMacros[name] = value
                orderedEqus.append(f".equ {name}, {value}")
            except Exception:
                pass
            continue

        # Enum detection
        if re.match(r"enum\b", line):
            insideEnum = True
            rawEnumLines.append(line)
            continue

        if insideEnum:
            rawEnumLines.append(line)
            if "}" in line:
                insideEnum = False

    # Second Pass: Evaluate enums
    enumValue = None
    for line in rawEnumLines:
        if "}" in line:
            enumValue = None
            continue

        entryMatch = re.match(r"\s*(\w+)\s*=\s*(.+?)(?:,)?$", line)
        if entryMatch:
            name, expr = entryMatch.groups()
            value = eval(expr, {"__builtins__": None}, definedMacros)
            definedMacros[name] = value
            orderedEqus.append(f".equ {name}, {value}")
            enumValue = value + 1
            continue

        incrementMatch = re.match(r"\s*(\w+),", line)
        if incrementMatch:
            name = incrementMatch.group(1)
            if enumValue is None:
                raise ValueError(f"Auto-increment value uninitialized for '{name}'")
            definedMacros[name] = enumValue
            orderedEqus.append(f".equ {name}, {enumValue}")
            enumValue += 1
            continue

    # Write output
    with outputFile.open('w') as file:
        file.write("\n".join(orderedEqus))
        file.write("\n")

    print(f"Conversion complete! {inputFile} ➝  {outputFile}")

# Example usage goes something like: python useful_scripts//convertHeader.py src/Tables/trainer_defines.h trainer_defines.s
def main():
    parser = argparse.ArgumentParser(description = "Convert C headers to ASM directives")
    parser.add_argument("input",  type = Path, help = "Path to the C header")
    parser.add_argument("output", type = Path, help = "Path for the ASM output")
    args = parser.parse_args()

    ConvertHeaderToASM(args.input, args.output)

main()
