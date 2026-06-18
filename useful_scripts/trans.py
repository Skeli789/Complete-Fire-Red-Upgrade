import os
import re
import time
from deep_translator import GoogleTranslator

translator = GoogleTranslator(source='en', target='es')

input_folder = "strings"
output_folder = "strings_es"

os.makedirs(output_folder, exist_ok=True)

special_codes = {
    r'\p': '[[P]]',
    r'\n': '[[N]]',
    r'\l': '[[L]]',
    r'\t': '[[T]]',
    r'\\': '[[BS]]',
}
reverse_codes = {v: k for k, v in special_codes.items()}

assignment_pattern = re.compile(r'^\s*[^=\s]+\s*=\s*.+$')
symbol_line = re.compile(r'^[-=_\s]+$')

def protect_codes(text):
    for code, placeholder in special_codes.items():
        text = text.replace(code, placeholder)
    return text

def restore_codes(text):
    for placeholder, code in reverse_codes.items():
        text = text.replace(placeholder, code)
    return text

def translate_text(text):
    protected = protect_codes(text)
    try:
        translated = translator.translate(protected)
        if translated is None:
            print("⚠ Translation returned None, keeping original.")
            translated = protected
    except Exception as e:
        print(f"⚠ Translation failed: {e}")
        translated = protected
    restored = restore_codes(translated)
    return restored.strip()

for filename in os.listdir(input_folder):
    if filename.endswith('.string'):
        input_path = os.path.join(input_folder, filename)
        output_path = os.path.join(output_folder, filename)
        done_marker = output_path + ".done"

        if os.path.exists(done_marker):
            print(f"✔ Skipping {filename} (already completed)")
            continue

        # Determine resume point
        resume_line = 0
        if os.path.exists(output_path):
            with open(output_path, 'r', encoding='utf-8') as f:
                resume_line = sum(1 for _ in f)

        print(f"\nTranslating {filename} (resuming from line {resume_line})...")

        with open(input_path, 'r', encoding='utf-8') as infile, \
             open(output_path, 'a', encoding='utf-8') as outfile:

            for idx, line in enumerate(infile):
                if idx < resume_line:
                    continue  # Skip already done lines

                stripped = line.strip()

                if (stripped.startswith('#') or
                    assignment_pattern.match(stripped) or
                    symbol_line.match(stripped)):

                    outfile.write(line)
                elif stripped:
                    print(f"→ Translating line {idx + 1}: {stripped[:50]}...")
                    translated_line = translate_text(stripped)
                    outfile.write(translated_line + '\n')
                else:
                    outfile.write(line)

        with open(done_marker, 'w') as marker:
            marker.write("done")

        print(f"✔ Finished {filename}")

print("\n✅ All translations complete! Check 'strings_es' folder.")
