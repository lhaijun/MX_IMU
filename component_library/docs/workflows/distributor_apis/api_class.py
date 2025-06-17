"""Class structure for distributor APIs."""

from abc import ABC, abstractmethod


class DistributorAPI(ABC):
    @property
    @abstractmethod
    def name(self) -> str:
        pass
