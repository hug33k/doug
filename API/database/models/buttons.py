from typing import Optional
from sqlmodel import Field, SQLModel


class ButtonBase(SQLModel):
    name: str
    soundId: Optional[int] = Field(default=None, foreign_key="sounds.id")
    iconId: Optional[int] = Field(default=None, foreign_key="icons.id")


class Buttons(ButtonBase, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)


class ButtonCreate(ButtonBase):
    pass


class ButtonRead(ButtonBase):
    id: int
    soundId: Optional[int]
    iconId: Optional[int]


class ButtonUpdate(SQLModel):
    name: Optional[str] = None
    soundId: Optional[int] = None
    iconId: Optional[int] = None
