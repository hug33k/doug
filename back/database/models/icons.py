from typing import Optional
from sqlmodel import Field, SQLModel


class IconBase(SQLModel):
	name: str
	path: Optional[str] = None


class Icons(IconBase, table=True):
	id: Optional[int] = Field(default=None, primary_key=True)


class IconCreate(IconBase):
	pass


class IconRead(IconBase):
	id: int


class IconUpdate(SQLModel):
	name: Optional[str] = None
	path: Optional[str] = None
