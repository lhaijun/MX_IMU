"""PCB Component PyDantic model for validation and abstraction.

Based on common KiCad component BOM structure.
"""

from pydantic import BaseModel, Field, field_validator


class PCBComponent(BaseModel):
    reference: str = Field(
        ..., description="Reference designator of the component"
    )
    value: str | None = Field(None, description="Value of the component")
    datasheet: str | None = Field(
        None, description="Datasheet URL / location of the component"
    )
    footprint: str | None = Field(
        None, description="Footprint of the component"
    )
    quantity: int = Field(
        ...,
        description="Quantity of the component required (alternatively "
        "represents availability if sourced from distributor data)",
    )
    do_not_populate: bool = Field(
        ..., description="Whether the component should not be populated"
    )
    manufacturer: str | None = Field(
        None, description="Manufacturer of the component"
    )
    manufacturer_part_number: str | None = Field(
        None, description="Manufacturer part number of the component"
    )
    distributor: str | None = Field(
        None, description="Distributor of the component"
    )
    distributor_part_number: str | None = Field(
        None, description="Distributor part number of the component"
    )
    distributor_link: str | None = Field(
        None, description="Distributor link of the component"
    )
    life_cycle_status: str | None = Field(
        None, description="Life cycle status of the component"
    )

    @field_validator("reference", mode="before")
    def validate_reference(cls, v):
        if not isinstance(v, str):
            raise ValueError("reference must be a valid string")
        if not v:
            raise ValueError("reference must not be empty")
        if not v[0].isalpha():
            raise ValueError("reference must start with a letter")
        return v

    @field_validator(
        "value",
        "datasheet",
        "footprint",
        "manufacturer",
        "manufacturer_part_number",
        "distributor",
        "distributor_part_number",
        "distributor_link",
        "life_cycle_status",
        mode="before",
    )
    def validate_optional_fields(cls, v, field):
        if v is not None and not isinstance(v, str):
            raise ValueError(f"{field.name} must be None or a valid string")
        return v

    @field_validator("quantity", mode="before")
    def validate_quantity(cls, v):
        if not isinstance(v, int):
            raise ValueError("quantity must be a valid integer")
        if v < 0:
            raise ValueError("quantity must be a unsigned integer")
        return v

    @field_validator("do_not_populate", mode="before")
    def validate_do_not_populate(cls, v):
        if not isinstance(v, bool):
            raise ValueError("do_not_populate must be a valid boolean")
        return v
