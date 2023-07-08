from sqlmodel import Session
from .models import Sounds, Icons, Buttons


def inject_sounds(session):
	sounds = [
		Sounds(name="My first sound", path="/sounds/1.mp3"),
		Sounds(name="Sound 2", path="/sounds/2.mp3"),
		Sounds(name="Third sound", path="/sounds/3.mp3")
	]
	for sound in sounds:
		session.add(sound)
	session.commit()
	return sounds


def inject_icons(session):
	icons = [
		Icons(name="My first icon", path="/icons/1.png"),
		Icons(name="Icon 2", path="/icons/2.jpg"),
		Icons(name="Third icon", path="/icons/2.gif")
	]
	for icon in icons:
		session.add(icon)
	session.commit()
	return icons


def inject_buttons(session, sounds, icons):
	buttons = [
		Buttons(name="My first button", soundId=sounds[0].id, iconId=icons[0].id),
		Buttons(name="Button 2", soundId=sounds[1].id),
		Buttons(name="Third button", iconId=icons[2].id),
		Buttons(name="Le bouton 4", soundId=sounds[0].id, iconId=icons[1].id),
		Buttons(name="El button 5", soundId=sounds[2].id, iconId=icons[1].id)
	]
	for button in buttons:
		session.add(button)
	session.commit()


def inject(engine):
	with Session(engine) as session:
		sounds = inject_sounds(session)
		icons = inject_icons(session)
		inject_buttons(session, sounds, icons)
		session.close()
