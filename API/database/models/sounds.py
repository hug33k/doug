from typing import Optional
from sqlmodel import Field, SQLModel


class SoundBase(SQLModel):
    name: str
    path: str


class Sounds(SoundBase, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)


class SoundCreate(SoundBase):
    pass


class SoundRead(SoundBase):
    id: int


class SoundUpdate(SQLModel):
    name: Optional[str] = None
    path: Optional[str] = None
