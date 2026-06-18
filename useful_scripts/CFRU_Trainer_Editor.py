#!/usr/bin/env python3
"""
Tkinter trainer editor for CFRU-expansion.

Edits:
  - src/Tables/trainer_data.c
  - src/Tables/trainer_parties.h

Preview images are read from BPRE0.gba using:
  - gTrainerFrontPicTable / gTrainerFrontPicPaletteTable
  - gMonFrontPicTable / gMonPaletteTable
"""

from __future__ import annotations

import os
import re
import struct
import sys
import tkinter as tk
from dataclasses import dataclass, field
from pathlib import Path
from tkinter import messagebox, ttk


ROOT = Path(__file__).resolve().parents[1]
TRAINER_DATA = ROOT / "src" / "Tables" / "trainer_data.c"
TRAINER_PARTIES = ROOT / "src" / "Tables" / "trainer_parties.h"
ROM_PATH = ROOT / "BPRE0.gba"
BPRE_LD = ROOT / "BPRE.ld"

PARTY_TYPE_TO_UNION = {
    "TrainerMonNoItemDefaultMoves": "NoItemDefaultMoves",
    "TrainerMonItemDefaultMoves": "ItemDefaultMoves",
    "TrainerMonNoItemCustomMoves": "NoItemCustomMoves",
    "TrainerMonItemCustomMoves": "ItemCustomMoves",
}
UNION_TO_PARTY_TYPE = {v: k for k, v in PARTY_TYPE_TO_UNION.items()}

PARTY_FLAGS = {
    "TrainerMonNoItemDefaultMoves": "0",
    "TrainerMonItemDefaultMoves": "PARTY_FLAG_HAS_ITEM",
    "TrainerMonNoItemCustomMoves": "PARTY_FLAG_CUSTOM_MOVES",
    "TrainerMonItemCustomMoves": "PARTY_FLAG_CUSTOM_MOVES | PARTY_FLAG_HAS_ITEM",
}

TRAINER_FIELDS = [
    "partyFlags",
    "trainerClass",
    "encounterMusic",
    "trainerPic",
    "trainerName",
    "items",
    "doubleBattle",
    "aiFlags",
]

AI_FLAG_CHOICES = [
    "AI_SCRIPT_CHECK_BAD_MOVE",
    "AI_SCRIPT_SEMI_SMART",
    "AI_SCRIPT_CHECK_GOOD_MOVE",
]

TEXT_TO_TOKEN = {
    " ": "_SPACE",
    "!": "_EXCLAMATION",
    "?": "_QUESTION",
    "'": "_APOSTROPHE",
    ".": "_PERIOD",
    "-": "_HYPHEN",
    "@": "_AT",
    "é": "_eACUTE",
    "0": "_0",
    "1": "_1",
    "2": "_2",
    "3": "_3",
    "4": "_4",
    "5": "_5",
    "6": "_6",
    "7": "_7",
    "8": "_8",
    "9": "_9",
}
for _ch in "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz":
    TEXT_TO_TOKEN[_ch] = f"_{_ch}"
TOKEN_TO_TEXT = {v: k for k, v in TEXT_TO_TOKEN.items()}
TOKEN_TO_TEXT.update({"_END": ""})


@dataclass
class MonEntry:
    enabled: bool = False
    fields: dict[str, str] = field(default_factory=dict)


@dataclass
class PartyDef:
    name: str
    struct_type: str
    start: int
    end: int
    mons: list[MonEntry]


@dataclass
class TrainerDef:
    ident: str
    start: int
    end: int
    fields: dict[str, str]
    party_array: str = ""
    union_member: str = "NoItemDefaultMoves"


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8", errors="replace")


def write_text(path: Path, text: str) -> None:
    path.write_text(text, encoding="utf-8")


def strip_comments(text: str) -> str:
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    text = re.sub(r"//.*", "", text)
    return text


def find_matching(text: str, open_index: int, open_char: str = "{", close_char: str = "}") -> int:
    depth = 0
    i = open_index
    while i < len(text):
        ch = text[i]
        if ch == open_char:
            depth += 1
        elif ch == close_char:
            depth -= 1
            if depth == 0:
                return i
        i += 1
    raise ValueError("Could not find matching brace")


def split_top_level_entries(body: str) -> list[str]:
    entries: list[str] = []
    start = None
    depth = 0
    for i, ch in enumerate(body):
        if start is None and not ch.isspace() and ch != ",":
            start = i
        if ch in "{[":
            depth += 1
        elif ch in "}]":
            depth -= 1
        elif ch == "," and depth == 0 and start is not None:
            part = body[start:i].strip()
            if part:
                entries.append(part)
            start = None
    if start is not None:
        part = body[start:].strip().rstrip(",")
        if part:
            entries.append(part)
    return entries


def extract_initializer_value(block: str, field_name: str) -> str:
    m = re.search(rf"\.{re.escape(field_name)}\s*=", block)
    if not m:
        return ""
    i = m.end()
    while i < len(block) and block[i].isspace():
        i += 1
    start = i
    depth = 0
    while i < len(block):
        ch = block[i]
        if ch in "{[(":
            depth += 1
        elif ch in "}])":
            if depth == 0:
                break
            depth -= 1
        elif ch == "," and depth == 0:
            break
        i += 1
    return block[start:i].strip()


def parse_c_defines(paths: list[Path], prefixes: tuple[str, ...]) -> tuple[dict[str, int], dict[int, str]]:
    names: dict[str, int] = {}
    reverse: dict[int, str] = {}
    define_re = re.compile(r"^\s*#define\s+([A-Z0-9_]+)\s+([0-9A-Fa-fx]+)\b")
    for path in paths:
        if not path.exists():
            continue
        for line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
            m = define_re.match(line)
            if not m or not m.group(1).startswith(prefixes):
                continue
            name, raw = m.groups()
            try:
                value = int(raw, 0)
            except ValueError:
                continue
            names[name] = value
            reverse.setdefault(value, name)
    return names, reverse


def parse_c_enums(path: Path, prefixes: tuple[str, ...]) -> tuple[dict[str, int], dict[int, str]]:
    names: dict[str, int] = {}
    reverse: dict[int, str] = {}
    if not path.exists():
        return names, reverse
    text = read_text(path)
    for enum_body in re.findall(r"enum\s*(?:[A-Za-z0-9_]*)?\s*\{(.*?)\}\s*;", text, flags=re.S):
        value = 0
        for raw_line in enum_body.splitlines():
            line = raw_line.split("//", 1)[0].strip().rstrip(",")
            if not line:
                continue
            if "=" in line:
                name, expr = [part.strip() for part in line.split("=", 1)]
                try:
                    value = int(expr, 0)
                except ValueError:
                    pass
            else:
                name = line.strip()
            if name.startswith(prefixes):
                names[name] = value
                reverse.setdefault(value, name)
            value += 1
    return names, reverse


def merge_constants(*pairs: tuple[dict[str, int], dict[int, str]]) -> tuple[dict[str, int], dict[int, str]]:
    names: dict[str, int] = {}
    reverse: dict[int, str] = {}
    for n, r in pairs:
        names.update(n)
        for value, name in r.items():
            reverse.setdefault(value, name)
    return names, reverse


def parse_braced_list(value: str) -> list[str]:
    value = value.strip()
    if value.startswith("{") and value.endswith("}"):
        value = value[1:-1]
    return [part.strip() for part in value.split(",") if part.strip()]


def decode_easy_text(value: str) -> str:
    chars: list[str] = []
    for token in parse_braced_list(value):
        if token == "_END":
            break
        chars.append(TOKEN_TO_TEXT.get(token, token.replace("_", "")))
    return "".join(chars)


def encode_easy_text(value: str) -> str:
    tokens = []
    for ch in value:
        tokens.append(TEXT_TO_TOKEN.get(ch, f"_{ch}"))
    tokens.append("_END")
    return "{" + ", ".join(tokens) + "}"


def expr_to_choice(expr: str, choices: list[str]) -> str:
    expr = expr.strip()
    if expr in choices:
        return expr
    return expr


def combine_flags(flags: list[str]) -> str:
    selected = [flag for flag in flags if flag]
    return " | ".join(selected) if selected else "0"


def parse_ld_symbols() -> dict[str, int]:
    text = read_text(BPRE_LD) if BPRE_LD.exists() else ""
    out: dict[str, int] = {}
    for name, value in re.findall(r"^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(0x[0-9A-Fa-f]+)", text, flags=re.M):
        out[name] = int(value, 16)
    return out


class RomSprites:
    def __init__(self, rom_path: Path):
        self.rom_path = rom_path
        self.rom = rom_path.read_bytes() if rom_path.exists() else b""
        self.syms = parse_ld_symbols()

    @staticmethod
    def gba_to_offset(ptr: int) -> int:
        return ptr & 0x01FFFFFF

    def read_u16(self, offset: int) -> int:
        return struct.unpack_from("<H", self.rom, offset)[0]

    def read_u32(self, offset: int) -> int:
        return struct.unpack_from("<I", self.rom, offset)[0]

    def lz77(self, ptr: int, expected: int | None = None) -> bytes:
        off = self.gba_to_offset(ptr)
        if off < 0 or off + 4 > len(self.rom):
            raise ValueError("Pointer outside ROM")
        if self.rom[off] != 0x10:
            if expected:
                return self.rom[off:off + expected]
            raise ValueError("Not LZ77 data")
        size = self.rom[off + 1] | (self.rom[off + 2] << 8) | (self.rom[off + 3] << 16)
        src = off + 4
        dst = bytearray()
        while len(dst) < size and src < len(self.rom):
            flags = self.rom[src]
            src += 1
            for bit in range(7, -1, -1):
                if len(dst) >= size:
                    break
                if not (flags & (1 << bit)):
                    dst.append(self.rom[src])
                    src += 1
                else:
                    b1 = self.rom[src]
                    b2 = self.rom[src + 1]
                    src += 2
                    count = (b1 >> 4) + 3
                    disp = ((b1 & 0xF) << 8) | b2
                    copy_src = len(dst) - disp - 1
                    for _ in range(count):
                        dst.append(dst[copy_src])
                        copy_src += 1
                        if len(dst) >= size:
                            break
        return bytes(dst)

    def read_sheet_entry(self, table: str, index: int) -> tuple[int, int]:
        base = self.syms.get(table)
        if not base:
            raise ValueError(f"Missing {table} in BPRE.ld")
        off = self.gba_to_offset(base) + index * 8
        return self.read_u32(off), self.read_u16(off + 4)

    def read_palette_entry(self, table: str, index: int) -> list[tuple[int, int, int]]:
        base = self.syms.get(table)
        if not base:
            raise ValueError(f"Missing {table} in BPRE.ld")
        off = self.gba_to_offset(base) + index * 8
        ptr = self.read_u32(off)
        raw = self.lz77(ptr, 32)
        if len(raw) < 32:
            raw = raw + b"\x00" * (32 - len(raw))
        colors = []
        for i in range(16):
            val = struct.unpack_from("<H", raw, i * 2)[0]
            r = (val & 0x1F) * 255 // 31
            g = ((val >> 5) & 0x1F) * 255 // 31
            b = ((val >> 10) & 0x1F) * 255 // 31
            colors.append((r, g, b))
        return colors

    def render_4bpp_64(self, pixels: bytes, palette: list[tuple[int, int, int]], scale: int = 2) -> tk.PhotoImage:
        w = h = 64
        rgb = [(240, 240, 240)] * (w * h)
        tile = 0
        for ty in range(8):
            for tx in range(8):
                base = tile * 32
                tile += 1
                if base + 32 > len(pixels):
                    continue
                for y in range(8):
                    for xpair in range(4):
                        byte = pixels[base + y * 4 + xpair]
                        for xoff, idx in ((0, byte & 0xF), (1, byte >> 4)):
                            x = tx * 8 + xpair * 2 + xoff
                            yy = ty * 8 + y
                            if idx:
                                rgb[yy * w + x] = palette[idx]
        if scale != 1:
            sw, sh = w * scale, h * scale
            scaled = []
            for y in range(h):
                row = []
                for x in range(w):
                    row.extend([rgb[y * w + x]] * scale)
                for _ in range(scale):
                    scaled.extend(row)
            rgb = scaled
            w, h = sw, sh
        return ppm_photo(rgb, w, h)

    def trainer_image(self, pic_id: int) -> tk.PhotoImage:
        ptr, size = self.read_sheet_entry("gTrainerFrontPicTable", pic_id)
        pal = self.read_palette_entry("gTrainerFrontPicPaletteTable", pic_id)
        pixels = self.lz77(ptr, size or 2048)
        return self.render_4bpp_64(pixels[:2048], pal, 2)

    def mon_image(self, species_id: int) -> tk.PhotoImage:
        ptr, size = self.read_sheet_entry("gMonFrontPicTable", species_id)
        pal = self.read_palette_entry("gMonPaletteTable", species_id)
        pixels = self.lz77(ptr, size or 2048)
        return self.render_4bpp_64(pixels[:2048], pal, 2)


def ppm_photo(rgb: list[tuple[int, int, int]], w: int, h: int) -> tk.PhotoImage:
    header = f"P6 {w} {h} 255\n".encode()
    data = bytearray(header)
    for r, g, b in rgb:
        data.extend((r, g, b))
    return tk.PhotoImage(data=bytes(data), format="PPM")


class ProjectData:
    def __init__(self):
        define_consts = parse_c_defines(
            [
                ROOT / "include" / "constants" / "species.h",
                ROOT / "include" / "constants" / "items.h",
                ROOT / "include" / "constants" / "moves.h",
                ROOT / "include" / "constants" / "trainers.h",
                ROOT / "include" / "constants" / "battle_ai.h",
            ],
            ("SPECIES_", "ITEM_", "MOVE_", "TRAINER_", "AI_", "NATURE_", "TYPE_"),
        )
        class_consts = parse_c_enums(ROOT / "include" / "constants" / "trainer_classes.h", ("CLASS_",))
        pic_consts = parse_c_enums(ROOT / "include" / "trainer_front_sprites.h", ("TRAINER_FRONT_PIC_",))
        self.constants, self.reverse = merge_constants(define_consts, class_consts, pic_consts)
        self.species_choices = self._choices("SPECIES_")
        self.item_choices = self._choices("ITEM_")
        self.trainer_pic_choices = self._choices("TRAINER_FRONT_PIC_")
        self.trainer_class_choices = self._choices("CLASS_")
        self.encounter_music_choices = self._choices("TRAINER_ENCOUNTER_MUSIC_")
        self.trainer_text = read_text(TRAINER_DATA)
        self.party_text = read_text(TRAINER_PARTIES)
        self.trainers = self.parse_trainers()
        self.parties = self.parse_parties()

    def reload(self):
        self.__init__()

    def const_value(self, expr: str) -> int | None:
        expr = expr.strip()
        if expr in self.constants:
            return self.constants[expr]
        try:
            return int(expr, 0)
        except ValueError:
            return None

    def _choices(self, prefix: str) -> list[str]:
        return sorted(
            [name for name in self.constants if name.startswith(prefix)],
            key=lambda name: (self.constants[name], name),
        )

    def parse_trainers(self) -> dict[str, TrainerDef]:
        out: dict[str, TrainerDef] = {}
        text = self.trainer_text
        for m in re.finditer(r"\[([A-Z0-9_]+)\]\s*=\s*\{", text):
            ident = m.group(1)
            start = m.start()
            open_i = text.find("{", m.end() - 1)
            close_i = find_matching(text, open_i)
            end = close_i + 1
            while end < len(text) and text[end] in " \t\r\n,":
                if text[end] == ",":
                    end += 1
                    break
                end += 1
            block = text[start:end]
            fields = {name: extract_initializer_value(block, name) for name in TRAINER_FIELDS}
            party_expr = extract_initializer_value(block, "party")
            pm = re.search(r"\.\s*([A-Za-z0-9_]+)\s*=\s*(sParty_[A-Za-z0-9_]+)", party_expr)
            union = pm.group(1) if pm else "NoItemDefaultMoves"
            party = pm.group(2) if pm else ""
            if not party:
                size_expr = extract_initializer_value(block, "partySize")
                sm = re.search(r"NELEMS\s*\(\s*(sParty_[A-Za-z0-9_]+)\s*\)", size_expr)
                if sm:
                    party = sm.group(1)
            out[ident] = TrainerDef(ident, start, end, fields, party, union)
        return out

    def parse_parties(self) -> dict[str, PartyDef]:
        out: dict[str, PartyDef] = {}
        text = self.party_text
        pat = re.compile(r"static\s+const\s+struct\s+([A-Za-z0-9_]+)\s+(sParty_[A-Za-z0-9_]+)\[\]\s*=\s*\{")
        for m in pat.finditer(text):
            struct_type, name = m.groups()
            open_i = text.find("{", m.end() - 1)
            close_i = find_matching(text, open_i)
            end = close_i + 1
            while end < len(text) and text[end] in " \t\r\n;":
                end += 1
            body = text[open_i + 1:close_i]
            mons = []
            for entry in split_top_level_entries(body):
                if "{" not in entry:
                    continue
                mon = MonEntry(True, {})
                for field_name in ["iv", "lvl", "species", "heldItem", "ability", "nature", "ivSpread", "evSpread", "teraType", "moves"]:
                    value = extract_initializer_value(entry, field_name)
                    if value:
                        mon.fields[field_name] = value
                mons.append(mon)
            out[name] = PartyDef(name, struct_type, m.start(), end, mons)
        return out

    def rebuild_trainer(self, tr: TrainerDef, party_type: str, party_name: str, party_size: int) -> str:
        fields = dict(tr.fields)
        fields["partyFlags"] = PARTY_FLAGS[party_type]
        union = PARTY_TYPE_TO_UNION[party_type]
        lines = [f"\t[{tr.ident}] = {{"]
        for name in TRAINER_FIELDS:
            value = fields.get(name, "")
            if name == "partyFlags":
                value = PARTY_FLAGS[party_type]
            if name == "trainerName" and not value:
                value = "{_END}"
            if name == "items" and not value:
                value = "{ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE}"
            if value:
                lines.append(f"\t    .{name} = {value},")
        lines.append(f"\t    .partySize = {party_size},")
        lines.append(f"\t    .party = {{.{union} = {party_name}}}")
        lines.append("\t},")
        return "\n".join(lines)

    def rebuild_party(self, party: PartyDef, party_type: str, mons: list[MonEntry]) -> str:
        lines = [f"static const struct {party_type} {party.name}[] = {{"]
        for mon in mons:
            if not mon.enabled:
                continue
            lines.append("    {")
            for key in party_fields_for_type(party_type):
                value = mon.fields.get(key, "").strip()
                if not value:
                    value = default_for_field(key)
                lines.append(f"        .{key} = {value},")
            lines.append("    },")
        lines.append("};")
        return "\n".join(lines)

    def save_selected(self, tr: TrainerDef, party: PartyDef, party_type: str, mons: list[MonEntry]) -> None:
        active = [m for m in mons if m.enabled]
        if not active:
            raise ValueError("The party must have at least 1 enabled Pokemon.")
        trainer_new = self.rebuild_trainer(tr, party_type, party.name, len(active))
        party_new = self.rebuild_party(party, party_type, mons)
        self.trainer_text = self.trainer_text[:tr.start] + trainer_new + self.trainer_text[tr.end:]
        self.party_text = self.party_text[:party.start] + party_new + self.party_text[party.end:]
        write_text(TRAINER_DATA, self.trainer_text)
        write_text(TRAINER_PARTIES, self.party_text)
        self.reload()


def party_fields_for_type(party_type: str) -> list[str]:
    if party_type == "TrainerMonNoItemDefaultMoves":
        return ["iv", "lvl", "species"]
    if party_type == "TrainerMonItemDefaultMoves":
        return ["iv", "lvl", "species", "heldItem"]
    if party_type == "TrainerMonNoItemCustomMoves":
        return ["iv", "lvl", "species", "moves"]
    return ["iv", "lvl", "species", "ability", "nature", "ivSpread", "evSpread", "heldItem", "moves", "teraType"]


def default_for_field(key: str) -> str:
    return {
        "iv": "0",
        "lvl": "5",
        "species": "SPECIES_NONE",
        "heldItem": "ITEM_NONE",
        "moves": "{MOVE_NONE, MOVE_NONE, MOVE_NONE, MOVE_NONE}",
        "ability": "Ability_Random_1_2",
        "nature": "NATURE_HARDY",
        "ivSpread": "{31, 31, 31, 31, 31, 31}",
        "evSpread": "{0, 0, 0, 0, 0, 0}",
        "teraType": "TYPE_BLANK",
    }.get(key, "0")


class TrainerEditor(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("CFRU Trainer Editor")
        self.geometry("1320x820")
        self.data = ProjectData()
        self.rom = RomSprites(ROM_PATH)
        self.current_trainer: TrainerDef | None = None
        self.current_party: PartyDef | None = None
        self.trainer_vars: dict[str, tk.StringVar] = {}
        self.party_name_var = tk.StringVar()
        self.item_vars: list[tk.StringVar] = []
        self.double_battle_var = tk.BooleanVar(value=False)
        self.ai_vars: dict[str, tk.BooleanVar] = {flag: tk.BooleanVar(value=False) for flag in AI_FLAG_CHOICES}
        self.mon_vars: list[dict[str, tk.Variable]] = []
        self.images: list[tk.PhotoImage] = []
        self._build_ui()
        self.populate_trainer_list()

    def _build_ui(self):
        root = ttk.PanedWindow(self, orient=tk.HORIZONTAL)
        root.pack(fill=tk.BOTH, expand=True)

        left = ttk.Frame(root, padding=8)
        root.add(left, weight=1)
        ttk.Label(left, text="Trainers").pack(anchor=tk.W)
        self.search_var = tk.StringVar()
        self.search_var.trace_add("write", lambda *_: self.populate_trainer_list())
        ttk.Entry(left, textvariable=self.search_var).pack(fill=tk.X, pady=(0, 6))
        self.trainer_list = tk.Listbox(left, exportselection=False)
        self.trainer_list.pack(fill=tk.BOTH, expand=True)
        self.trainer_list.bind("<<ListboxSelect>>", self.on_trainer_select)

        right = ttk.Frame(root, padding=8)
        root.add(right, weight=5)

        top = ttk.Frame(right)
        top.pack(fill=tk.X)
        form = ttk.LabelFrame(top, text="Trainer data", padding=8)
        form.pack(side=tk.LEFT, fill=tk.X, expand=True)

        self.trainer_vars["partyFlags"] = tk.StringVar()
        self.trainer_vars["trainerName"] = tk.StringVar()
        self.trainer_vars["trainerClass"] = tk.StringVar()
        self.trainer_vars["encounterMusic"] = tk.StringVar()
        self.trainer_vars["trainerPic"] = tk.StringVar()
        self.trainer_vars["aiFlags"] = tk.StringVar()

        ttk.Label(form, text="Trainer Name").grid(row=0, column=0, sticky=tk.W, padx=(0, 4), pady=2)
        ttk.Entry(form, textvariable=self.trainer_vars["trainerName"], width=34).grid(row=0, column=1, sticky=tk.EW, pady=2)

        ttk.Label(form, text="Trainer Class").grid(row=0, column=2, sticky=tk.W, padx=(10, 4), pady=2)
        ttk.Combobox(form, textvariable=self.trainer_vars["trainerClass"], values=self.data.trainer_class_choices, width=34).grid(row=0, column=3, sticky=tk.EW, pady=2)

        ttk.Label(form, text="Trainer Pic").grid(row=1, column=0, sticky=tk.W, padx=(0, 4), pady=2)
        pic_combo = ttk.Combobox(form, textvariable=self.trainer_vars["trainerPic"], values=self.data.trainer_pic_choices, width=34)
        pic_combo.grid(row=1, column=1, sticky=tk.EW, pady=2)
        pic_combo.bind("<<ComboboxSelected>>", lambda *_: self.update_trainer_preview())
        self.trainer_vars["trainerPic"].trace_add("write", lambda *_: self.update_trainer_preview())

        ttk.Label(form, text="Encounter Music").grid(row=1, column=2, sticky=tk.W, padx=(10, 4), pady=2)
        ttk.Combobox(form, textvariable=self.trainer_vars["encounterMusic"], values=self.data.encounter_music_choices, width=34).grid(row=1, column=3, sticky=tk.EW, pady=2)

        ttk.Label(form, text="Items").grid(row=2, column=0, sticky=tk.W, padx=(0, 4), pady=2)
        items_frame = ttk.Frame(form)
        items_frame.grid(row=2, column=1, columnspan=3, sticky=tk.EW, pady=2)
        for i in range(4):
            var = tk.StringVar(value="ITEM_NONE")
            self.item_vars.append(var)
            ttk.Combobox(items_frame, textvariable=var, values=self.data.item_choices, width=28).pack(side=tk.LEFT, padx=(0, 4))

        ttk.Checkbutton(form, text="Double Battle", variable=self.double_battle_var).grid(row=3, column=0, sticky=tk.W, pady=2)

        ttk.Label(form, text="AI Flags").grid(row=3, column=1, sticky=tk.W, pady=2)
        ai_frame = ttk.Frame(form)
        ai_frame.grid(row=3, column=2, columnspan=2, sticky=tk.W, pady=2)
        for flag in AI_FLAG_CHOICES:
            ttk.Checkbutton(ai_frame, text=flag.replace("AI_SCRIPT_", ""), variable=self.ai_vars[flag]).pack(side=tk.LEFT, padx=(0, 10))

        ttk.Label(form, text="Party array").grid(row=4, column=0, sticky=tk.W, padx=(0, 4), pady=2)
        ttk.Entry(form, textvariable=self.party_name_var, state="readonly").grid(row=4, column=1, columnspan=3, sticky=tk.EW, pady=2)

        self.trainer_vars["items"] = tk.StringVar()
        self.trainer_vars["doubleBattle"] = tk.StringVar()
        form.columnconfigure(1, weight=1)
        form.columnconfigure(3, weight=1)

        preview = ttk.LabelFrame(top, text="Trainer sprite", padding=8)
        preview.pack(side=tk.RIGHT, padx=(8, 0))
        self.trainer_img_label = ttk.Label(preview, text="no preview", anchor=tk.CENTER)
        self.trainer_img_label.pack()

        options = ttk.Frame(right)
        options.pack(fill=tk.X, pady=8)
        ttk.Label(options, text="Party struct").pack(side=tk.LEFT)
        self.party_type_var = tk.StringVar(value="TrainerMonNoItemDefaultMoves")
        self.party_type = ttk.Combobox(options, textvariable=self.party_type_var, values=list(PARTY_TYPE_TO_UNION), state="readonly", width=32)
        self.party_type.pack(side=tk.LEFT, padx=6)
        self.party_type.bind("<<ComboboxSelected>>", lambda *_: self.refresh_party_field_states())
        ttk.Button(options, text="Save trainer and party", command=self.save_current).pack(side=tk.RIGHT)
        ttk.Button(options, text="Reload files", command=self.reload_all).pack(side=tk.RIGHT, padx=6)
        ttk.Label(
            right,
            text="TRAINERS_WITH_EVS note: in common/custom parties without ItemCustom, the iv field may be used as an index into gTrainersWithEvsSpreads. In ItemCustom, ivSpread/evSpread are edited inline.",
            foreground="#555",
        ).pack(fill=tk.X, pady=(0, 6))

        party = ttk.LabelFrame(right, text="Party (3 top, 3 bottom)", padding=8)
        party.pack(fill=tk.BOTH, expand=True)
        for i in range(6):
            frame = ttk.LabelFrame(party, text=f"Slot {i + 1}", padding=6)
            frame.grid(row=i // 3, column=i % 3, sticky=tk.NSEW, padx=4, pady=4)
            party.columnconfigure(i % 3, weight=1)
            party.rowconfigure(i // 3, weight=1)
            self.create_mon_editor(frame, i)

    def create_mon_editor(self, parent: ttk.Frame, index: int):
        vars_: dict[str, tk.Variable] = {"enabled": tk.BooleanVar(value=False)}
        self.mon_vars.append(vars_)
        ttk.Checkbutton(parent, text="Use slot", variable=vars_["enabled"], command=self.update_mon_previews).grid(row=0, column=0, columnspan=2, sticky=tk.W)
        sprite_frame = ttk.LabelFrame(parent, text="Preview", padding=4)
        sprite_frame.grid(row=0, column=2, rowspan=11, sticky=tk.N, padx=(10, 0))
        img = ttk.Label(sprite_frame, text="sprite", anchor=tk.CENTER, width=18)
        img.pack()
        vars_["_image_label"] = img  # type: ignore[assignment]
        fields = ["species", "lvl", "iv", "heldItem", "ability", "nature", "ivSpread", "evSpread", "moves", "teraType"]
        for r, key in enumerate(fields, start=1):
            label = "iv / EV spread #" if key == "iv" else key
            ttk.Label(parent, text=label).grid(row=r, column=0, sticky=tk.W)
            var = tk.StringVar()
            vars_[key] = var
            if key == "species":
                ent = ttk.Combobox(parent, textvariable=var, values=self.data.species_choices, width=34)
            elif key == "heldItem":
                ent = ttk.Combobox(parent, textvariable=var, values=self.data.item_choices, width=34)
            else:
                ent = ttk.Entry(parent, textvariable=var, width=34)
            ent.grid(row=r, column=1, sticky=tk.EW, pady=1)
            vars_[f"_{key}_entry"] = ent  # type: ignore[assignment]
            if key == "species":
                var.trace_add("write", lambda *_: self.update_mon_previews())
        parent.columnconfigure(1, weight=1)

    def populate_trainer_list(self):
        query = self.search_var.get().upper()
        self.trainer_list.delete(0, tk.END)
        self.trainer_order = []
        for ident in sorted(self.data.trainers, key=lambda k: self.data.constants.get(k, 999999)):
            tr = self.data.trainers[ident]
            searchable = f"{ident} {tr.party_array}".upper()
            if query and query not in searchable:
                continue
            label = f"{self.data.constants.get(ident, 0):04d}  {ident}  ->  {tr.party_array}"
            self.trainer_order.append(ident)
            self.trainer_list.insert(tk.END, label)

    def on_trainer_select(self, _event=None):
        sel = self.trainer_list.curselection()
        if not sel:
            return
        ident = self.trainer_order[sel[0]]
        tr = self.data.trainers[ident]
        party = self.data.parties.get(tr.party_array)
        if party is None:
            self.data.reload()
            tr = self.data.trainers.get(ident, tr)
            party = self.data.parties.get(tr.party_array)
        if party is None:
            messagebox.showwarning("Party not found", f"Could not find {tr.party_array} in trainer_parties.h")
            return
        self.current_trainer = tr
        self.current_party = party
        self.party_name_var.set(party.name)
        self.trainer_vars["partyFlags"].set(tr.fields.get("partyFlags", ""))
        self.trainer_vars["trainerName"].set(decode_easy_text(tr.fields.get("trainerName", "{_END}")))
        self.trainer_vars["trainerClass"].set(expr_to_choice(tr.fields.get("trainerClass", ""), self.data.trainer_class_choices))
        self.trainer_vars["encounterMusic"].set(expr_to_choice(tr.fields.get("encounterMusic", ""), self.data.encounter_music_choices))
        self.trainer_vars["trainerPic"].set(expr_to_choice(tr.fields.get("trainerPic", ""), self.data.trainer_pic_choices))
        self.trainer_vars["items"].set(tr.fields.get("items", "{ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE}"))
        item_values = parse_braced_list(self.trainer_vars["items"].get())
        item_values = (item_values + ["ITEM_NONE", "ITEM_NONE", "ITEM_NONE", "ITEM_NONE"])[:4]
        for var, value in zip(self.item_vars, item_values):
            var.set(value)
        self.trainer_vars["doubleBattle"].set(tr.fields.get("doubleBattle", "FALSE"))
        self.double_battle_var.set(tr.fields.get("doubleBattle", "FALSE").strip() == "TRUE")
        self.trainer_vars["aiFlags"].set(tr.fields.get("aiFlags", "0"))
        ai_expr = tr.fields.get("aiFlags", "0")
        for flag, var in self.ai_vars.items():
            var.set(flag in ai_expr)
        self.party_type_var.set(party.struct_type)
        for i in range(6):
            vars_ = self.mon_vars[i]
            mon = party.mons[i] if i < len(party.mons) else MonEntry(False, {})
            vars_["enabled"].set(mon.enabled)
            for key in ["species", "lvl", "iv", "heldItem", "ability", "nature", "ivSpread", "evSpread", "moves", "teraType"]:
                vars_[key].set(mon.fields.get(key, default_for_field(key)))
        self.update_trainer_preview()
        self.update_mon_previews()
        self.refresh_party_field_states()

    def refresh_party_field_states(self):
        ptype = self.party_type_var.get()
        active = set(party_fields_for_type(ptype))
        for vars_ in self.mon_vars:
            for key in ["heldItem", "ability", "nature", "ivSpread", "evSpread", "moves", "teraType"]:
                entry = vars_.get(f"_{key}_entry")
                if entry:
                    entry.configure(state="normal" if key in active else "disabled")

    def update_trainer_preview(self):
        if not self.current_trainer:
            return
        value = self.trainer_vars["trainerPic"].get()
        pic_id = self.data.const_value(value)
        if pic_id is None:
            self.trainer_img_label.configure(text="invalid pic", image="")
            return
        try:
            img = self.rom.trainer_image(pic_id)
            self.images.append(img)
            self.trainer_img_label.configure(image=img, text="")
        except Exception as exc:
            self.trainer_img_label.configure(text=f"no preview\n{exc}", image="")

    def update_mon_previews(self):
        for vars_ in self.mon_vars:
            label = vars_["_image_label"]
            if not vars_["enabled"].get():
                label.configure(text="empty", image="")
                continue
            species = self.data.const_value(vars_["species"].get())
            if species is None:
                label.configure(text="invalid species", image="")
                continue
            try:
                img = self.rom.mon_image(species)
                self.images.append(img)
                label.configure(image=img, text="")
            except Exception as exc:
                label.configure(text=f"no sprite\n{exc}", image="")

    def collect_current(self) -> tuple[TrainerDef, PartyDef, str, list[MonEntry]]:
        if not self.current_trainer or not self.current_party:
            raise ValueError("Select a trainer.")
        tr = self.current_trainer
        tr.fields = dict(tr.fields)
        tr.fields["trainerName"] = encode_easy_text(self.trainer_vars["trainerName"].get().strip())
        tr.fields["trainerClass"] = self.trainer_vars["trainerClass"].get().strip()
        tr.fields["encounterMusic"] = self.trainer_vars["encounterMusic"].get().strip()
        tr.fields["trainerPic"] = self.trainer_vars["trainerPic"].get().strip()
        tr.fields["items"] = "{" + ", ".join(var.get().strip() or "ITEM_NONE" for var in self.item_vars) + "}"
        tr.fields["doubleBattle"] = "TRUE" if self.double_battle_var.get() else "FALSE"
        tr.fields["aiFlags"] = combine_flags([flag if var.get() else "" for flag, var in self.ai_vars.items()])
        ptype = self.party_type_var.get()
        mons = []
        for vars_ in self.mon_vars:
            mon = MonEntry(bool(vars_["enabled"].get()), {})
            for key in party_fields_for_type(ptype):
                mon.fields[key] = vars_[key].get().strip()
            mons.append(mon)
        return tr, self.current_party, ptype, mons

    def save_current(self):
        try:
            tr, party, ptype, mons = self.collect_current()
            self.data.save_selected(tr, party, ptype, mons)
            messagebox.showinfo("Saved", "trainer_data.c and trainer_parties.h were updated.")
            self.populate_trainer_list()
        except Exception as exc:
            messagebox.showerror("Save error", str(exc))

    def reload_all(self):
        self.data.reload()
        self.populate_trainer_list()
        messagebox.showinfo("Reloaded", "Files reloaded from disk.")


def main() -> int:
    app = TrainerEditor()
    app.mainloop()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
