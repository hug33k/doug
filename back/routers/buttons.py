from typing import List, Union
from fastapi import APIRouter, Query, status, HTTPException
from database import get_session, select
from database.models import ButtonCreate, ButtonRead, ButtonUpdate, Buttons



router = APIRouter(
	prefix="/buttons",
	tags=["buttons"],
)


@router.get("", response_model=List[ButtonRead])
async def get_buttons():
	with get_session() as session:
		buttons = session.exec(select(Buttons)).all()
		return buttons


@router.get("/{button_id}", response_model=Union[ButtonRead, None])
async def get_button(button_id: int = Query()):
	with get_session() as session:
		button = session.get(Buttons, button_id)
		if not button:
			raise HTTPException(status_code=404, detail="Button not found")
		return button


@router.post("", response_model=ButtonRead, status_code=status.HTTP_201_CREATED)
async def add_button(input_button: ButtonCreate):
	with get_session() as session:
		button = Buttons.from_orm(input_button)
		session.add(button)
		session.commit()
		session.refresh(button)
		return button


@router.post("/{button_id}", response_model=ButtonRead)
async def update_button(input_button: ButtonUpdate, button_id: int = Query()):
	with get_session() as session:
		print(input_button)
		button = session.get(Buttons, button_id)
		if not button:
			raise HTTPException(status_code=404, detail="Button not found")
		input_button_dict = input_button.dict(exclude_unset=True)
		for key, value in input_button_dict.items():
			setattr(button, key, value)
		session.add(button)
		session.commit()
		session.refresh(button)
		return button


@router.delete("/{button_id}", status_code=status.HTTP_204_NO_CONTENT)
async def remove_button(button_id: int = Query()):
	with get_session() as session:
		button = session.get(Buttons, button_id)
		if not button:
			raise HTTPException(status_code=404, detail="Button not found")
		session.delete(button)
		session.commit()
		return
