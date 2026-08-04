import sys

def generate_arrays(filename):
    # Initialize arrays with 128 null bytes (Standard Set 1 range)
    normal_map = ['\\0'] * 128
    shift_map = ['\\0'] * 128

    # Special character mapping
    special_chars = {
        "Backspace": "\\b",
        "Tab": "\\t",
        "Enter": "\\n",
        "Spacebar": " ",
    }

    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if not line: continue

            parts = line.split('\t')
            # Clean up empty strings caused by multiple tabs
            parts = [p.strip() for p in parts if p.strip()]

            if len(parts) < 3: continue

            # 1. Parse Scan Code (Make)
            # We take the Make code. If it's "E0 1C", we flag it as extended.
            raw_code = parts[1].split('/')[0].strip()

            is_extended = "E0" in raw_code or "E1" in raw_code
            # Get the actual hex value (the last byte)
            hex_val = int(raw_code.split()[-1], 16)

            # For the basic echo, we only map non-extended keys
            if is_extended:
                continue

            # 2. Parse Characters
            # The char part might be "a  A" or "Backspace" or ";  :"
            chars_part = parts[2]

            base_char = ""
            upper_char = ""

            if chars_part in special_chars:
                base_char = upper_char = special_chars[chars_part]
            elif "  " in chars_part:
                # Splits "a  A" or ";  :"
                cp = [c for c in chars_part.split(' ') if c]
                base_char = cp[0]
                upper_char = cp[1] if len(cp) > 1 else cp[0]
            else:
                # Single character or thing we don't handle yet
                base_char = upper_char = chars_part[0] if chars_part else "\\0"

            # 3. Clean up quotes for C
            def c_escape(c):
                if c == "'": return "\\'"
                if c == "\\": return "\\\\"
                return c

            if hex_val < 128:
                normal_map[hex_val] = c_escape(base_char)
                shift_map[hex_val] = c_escape(upper_char)

    # Output the C code
    print("const u8 ps2_set[128] = {")
    print("    " + ", ".join(f"'{c}'" if len(c) == 1 or c.startswith('\\') else f"'{c}'" for c in normal_map))
    print("};\n")

    print("const u8 shift_ps2_set[128] = {")
    print("    " + ", ".join(f"'{c}'" if len(c) == 1 or c.startswith('\\') else f"'{c}'" for c in shift_map))
    print("};")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        generate_arrays(sys.argv[1])
    else:
        print("Usage: python gen_map.py scancodes.txt")
