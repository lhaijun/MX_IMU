"""Mouser API Script.

Swagger Docs: https://api.mouser.com/api/docs/ui/index#.
Docs Docs: https://api.mouser.com/api/docs/V1.
"""

import os
import re

import requests

from component_class import PCBComponent
from api_class import DistributorAPI

# Base API url.
BASE_URL = "https://api.mouser.com/"

# Load environment variables.
API_VERSION = os.getenv("MOUSER_API_VERSION")
assert API_VERSION is not None, "Missing MOUSER_API_VERSION from env."
API_KEY = os.getenv("MOUSER_API_KEY")
assert API_KEY is not None, "Missing MOUSER_API_KEY from env."


class Mouser(DistributorAPI):
    @property
    def name(self) -> str:
        return "Mouser"

    @staticmethod
    def search_part(part_number: str) -> list[PCBComponent]:
        """Search for a part number on the Mouser API.

        Args:
            part_number: Mouser part number to search for.

        Returns:
            List of PCBComponent objects, max 50 search results.

        Raises:
            RuntimeWarning: For failed non-200 HTTPS code.
        """

        def extract_json_response(json_response: dict) -> list[PCBComponent]:
            """Extract list of PCBComponent objects from API json response.

            Args:
                json_response: Value of POST / GET request's response.json().

            Returns:
                List of PCBComponent objects
            """
            components = []

            try:
                results = json_response["SearchResults"]
                parts = [] if results is None else results["Parts"]
            except KeyError:
                raise RuntimeWarning("Unexpected json response body")

            for part in parts:
                # Extract availability with regex.
                match = re.search(r"(\d+) In Stock", part["Availability"])
                availability = int(match.group(1)) if match else 0

                component = PCBComponent(
                    reference="DISTRIBUTOR_MOUSER_DATA",
                    value=None,
                    datasheet=part["DataSheetUrl"],
                    footprint=None,
                    quantity=availability,
                    do_not_populate=False,
                    manufacturer=part["Manufacturer"],
                    manufacturer_part_number=part["ManufacturerPartNumber"],
                    distributor="Mouser",
                    distributor_part_number=part["MouserPartNumber"],
                    distributor_link=part["ProductDetailUrl"],
                    life_cycle_status=part["LifecycleStatus"],
                )
                components.append(component)

            return components

        # Prepare URL and params.
        url = f"{BASE_URL}api/v{API_VERSION}/search/partnumber"

        # Prepare headers and POST request data payload.
        headers = {
            "accept": "application/json",
            "Content-Type": "application/json",
        }
        data = {
            "SearchByPartRequest": {
                "mouserPartNumber": part_number,
                "partSearchOptions": "",
            }
        }

        # Make the POST request.
        response = requests.post(
            url, headers=headers, json=data, params={"apiKey": API_KEY}
        )

        # Check if the request was successful.
        if response.status_code == 200:
            result = response.json()
            return extract_json_response(json_response=result)
        else:
            raise RuntimeWarning(
                f"Failed to fetch data: {response.status_code}"
            )
