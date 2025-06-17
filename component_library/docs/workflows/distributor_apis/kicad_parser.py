"""KiCad custom library parsers."""

import os
import re

from component_class import PCBComponent


def is_kicad_symbol_file(file_name: str) -> bool:
    """
    Determine if a given file is a KiCad symbol file.

    Args:
        file_name: The name of the file to check.

    Returns:
        bool: True if the file is a KiCad symbol file, False otherwise.
    """
    if file_name.endswith(".kicad_sym"):
        return True
    return False


def is_kicad_footprint_file(file_name: str) -> bool:
    """
    Determine if a given file is a KiCad footprint file.

    Args:
        file_name: The name of the file to check.

    Returns:
        bool: True if the file is a KiCad footprint file, False otherwise.
    """
    if file_name.endswith(".kicad_mod"):
        return True
    return False


def is_kicad_footprint_dir(directory: str) -> bool:
    """
    Determine if a given directory is a KiCad footprint directory.

    Args:
        directory: The name of the file to check.

    Returns:
        bool: True if the file is a KiCad footprint directory, False otherwise.
    """

    if directory.endswith(".pretty"):
        for root, dirs, files in os.walk(directory):
            for file in files:
                file_path = os.path.join(root, file)
                if is_kicad_footprint_file(file_path):
                    return True
    return False


def library_types_dict(
    footprints: list[str | PCBComponent], symbols: list[str | PCBComponent]
) -> dict[str, list[str] | list[PCBComponent]]:
    """Get a format a dict with standardized keys of all library types.

    Args:
        footprints: List of footprints to pass.
        symbols: List of symbols to pass.

    Returns:
        Format:
        {
            "footprints": [ ... ],
            "symbols": [ ... ]
        }
    """
    return {"footprints": footprints, "symbols": symbols}


def find_kicad_libray_files(
    starting_dir: str | None = ".", exclude_dirs: list[str] = None
) -> dict[str, list[str]]:
    """Find all KiCad library files starting at a given directory.

    Args:
        starting_dir: Starting directory to search, defaults to current.
        exclude_dirs: List of directories to ignore.

    Returns:
        A dict with values of list os library files.

    Notes:
        A maximum search directory search limit is set within the function.
    """
    search_limit = 50  # Maximum number of directories to look in.

    if not isinstance(exclude_dirs, list):
        exclude_dirs = []  # Ensure a valid list.

    if not isinstance(starting_dir, str):  # Param secondary setter.
        starting_dir = ""

    symbol_files = []
    footprint_files = []

    # Find all KiCad symbols in current directory.
    for i, (root, dirs, files) in enumerate(os.walk(starting_dir)):
        if i > search_limit:
            return library_types_dict(
                footprints=footprint_files, symbols=symbol_files
            )

        dirs[:] = [d for d in dirs if d not in exclude_dirs]  # Exclude dirs.

        for file in files:
            if not is_kicad_footprint_dir(root) and is_kicad_symbol_file(file):
                symbol_files.append(os.path.join(root, file))
            if is_kicad_footprint_dir(root) and is_kicad_footprint_file(file):
                footprint_files.append(os.path.join(root, file))

    return library_types_dict(footprints=footprint_files, symbols=symbol_files)


def get_property(text: str, property_name: str) -> None | str:
    """Extract property values from KiCad component library file text.

    Args:
        text: Text to search within.
        property_name: Property name to search match for.

    Returns:
        Matched property value or None if not found.
    """
    match = re.search(rf'property\s+"{property_name}"\s+"([^"]+)"', text)
    if match:
        return match.group(1)
    return None


def parse_symbol(symbol_file_path: str) -> PCBComponent:
    """Parse KiCad symbol file and create a PCBComponent object.

    Returns:
        List of PCBComponent objects.
    """
    with open(symbol_file_path, "r") as f:
        text = f.read()
        return PCBComponent(
            reference=get_property(text, "Reference"),
            value=get_property(text, "Value"),
            datasheet=get_property(text, "Datasheet"),
            footprint=get_property(text, "Footprint"),
            quantity=0,
            do_not_populate=False,
            manufacturer=get_property(text, "Manufacturer"),
            manufacturer_part_number=get_property(
                text, "Manufacturer Part Number"
            ),
            distributor=get_property(text, "Distributor"),
            distributor_part_number=get_property(
                text, "Distributor Part Number"
            ),
            distributor_link=get_property(text, "Distributor Link"),
        )


def parse_footprint(footprint_file_path: str) -> PCBComponent:
    """Parse KiCad footprint file and create a PCBComponent object.

    Returns:
        List of PCBComponent objects.
    """
    with open(footprint_file_path, "r") as f:
        text = f.read()
        return PCBComponent(
            reference=get_property(text, "Reference"),
            value=get_property(text, "Value"),
            datasheet=get_property(text, "Datasheet"),
            footprint=get_property(text, "Footprint"),
            quantity=0,
            do_not_populate=False,
            manufacturer=get_property(text, "Manufacturer"),
            manufacturer_part_number=get_property(
                text, "Manufacturer Part Number"
            ),
        )


def parse_library(
    starting_dir: str = None, exclude_dirs: list[str] = None
) -> dict[str, list[PCBComponent]]:
    """Parse the entire KiCad library and create PCBComponent objects.

    Args:
        starting_dir: Starting directory to search, defaults to current.
        exclude_dirs: List of directories to ignore.

    Returns:
        A dict with PCBComponent objects.
    """
    symbols = []
    footprints = []

    library_files = find_kicad_libray_files(
        starting_dir=starting_dir, exclude_dirs=exclude_dirs
    )

    for symbol_file_path in library_files["symbols"]:
        symbols.append(parse_symbol(symbol_file_path))
    for footprint_file_path in library_files["footprints"]:
        footprints.append(parse_footprint(footprint_file_path))

    return library_types_dict(footprints=footprints, symbols=symbols)
