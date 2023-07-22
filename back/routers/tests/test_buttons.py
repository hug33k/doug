from fastapi.testclient import TestClient
from fastapi.exceptions import RequestValidationError
from fastapi import HTTPException
import pytest
from tests.misc import check_key_in_dict
from ..buttons import router


client = TestClient(router)


def assert_button_read_class(button):
	assert isinstance(button, dict)
	check_key_in_dict(button, "id", int)
	check_key_in_dict(button, "soundId", int, True)
	check_key_in_dict(button, "iconId", int, True)


def test__get_buttons__success():
	response_success = client.get("/buttons")
	data = response_success.json()
	assert response_success.status_code == 200
	assert isinstance(data, list)
	for item in data:
		assert_button_read_class(item)


def test__get_button__success():
	response_success = client.get("/buttons/1")
	data = response_success.json()
	assert response_success.status_code == 200
	assert_button_read_class(data)


def test__get_button__not_found():
	with pytest.raises(HTTPException) as exception:
		client.get("/buttons/999")
	assert exception.value.status_code == 404
	assert exception.value.detail == "Button not found"


def test__get_button__invalid_format():
	with pytest.raises(RequestValidationError) as exception:
		client.get("/buttons/a")
	assert """path -> button_id
  value is not a valid integer (type=type_error.integer)""" in str(exception.value)


def test__add_button__success():
	payload_default = {
		"name": "Button test",
		"soundId": 1,
		"iconId": 1
	}
	payload__no_sound = {
		"name": "Button test no sound",
		"soundId": None,
		"iconId": 1
	}
	payload__no_icon = {
		"name": "Button test no icon",
		"soundId": 1,
		"iconId": None
	}
	payload__no_sound_no_icon = {
		"name": "Button test no sound no icon",
		"soundId": None,
		"iconId": None
	}
	payload__non_required_fields = {
		"name": "Button test non required fields",
		"soundId": 1,
		"iconId": 1,
		"not_a_field": True
	}

	payloads = [
		payload_default,
		payload__no_sound,
		payload__no_icon,
		payload__no_sound_no_icon,
		payload__non_required_fields
	]

	for payload in payloads:
		response_success = client.post("/buttons", json=payload)
		data = response_success.json()
		assert response_success.status_code == 201
		assert_button_read_class(data)
		assert data["name"] == payload["name"]
		assert data["soundId"] == payload["soundId"]
		assert data["iconId"] == payload["iconId"]
		assert not "not_a_field" in data


def test__add_button__invalid_format():
	payload__invalid_sound = {
		"name": "Button test",
		"soundId": "test",
		"iconId": None
	}
	payload__invalid_icon = {
		"name": "Button test",
		"soundId": None,
		"iconId": "test"
	}
	payload__invalid_name = {
		"name": None,
		"soundId": None,
		"iconId": None
	}
	payload__no_body = {}

	payloads = [
		payload__invalid_sound,
		payload__invalid_icon,
		payload__invalid_name,
		payload__no_body
	]
	errors = [
		"""body -> soundId
  value is not a valid integer (type=type_error.integer)""",
		"""body -> iconId
  value is not a valid integer (type=type_error.integer)""",
		"""body -> name
  none is not an allowed value (type=type_error.none.not_allowed)""",
		"""body -> name
  field required (type=value_error.missing)""",
	]

	for (payload, error) in zip(payloads, errors):
		with pytest.raises(RequestValidationError) as exception:
			client.post("/buttons", json=payload)
		assert error in str(exception.value)


def test__update_button__success():
	payload_default = {
		"name": "Button test",
		"soundId": 1,
		"iconId": 1
	}
	payload__no_sound = {
		"name": "Button test no sound",
		"iconId": 1
	}
	payload__no_icon = {
		"name": "Button test no icon",
		"soundId": 1,
	}
	payload__no_sound_no_icon = {
		"name": "Button test no sound no icon",
	}
	payload__no_name = {
		"soundId": 1,
		"iconId": 1
	}
	payload__non_required_fields = {
		"name": "Button test non required fields",
		"soundId": 1,
		"iconId": 1,
		"not_a_field": True
	}
	payload__no_body = {}

	payloads = [
		payload_default,
		payload__no_sound,
		payload__no_icon,
		payload__no_sound_no_icon,
		payload__no_name,
		payload__non_required_fields,
		payload__no_body
	]

	fields = ["name", "soundId", "iconId"]

	for payload in payloads:
		response_success = client.post("/buttons/1", json=payload)
		data = response_success.json()
		assert response_success.status_code == 200
		assert_button_read_class(data)
		for field in fields:
			if field in payload:
				assert data[field] == payload[field]
		assert not "not_a_field" in data


def test__update_button__not_found():
	payload = {
		"name": "Button test",
		"soundId": 1,
		"iconId": 1
	}
	with pytest.raises(HTTPException) as exception:
		client.post("/buttons/999", json=payload)
	assert exception.value.status_code == 404
	assert exception.value.detail == "Button not found"


def test__update_button__invalid_format():
	payload__invalid_sound = {
		"name": "Button test",
		"soundId": "test",
		"iconId": None
	}
	payload__invalid_icon = {
		"name": "Button test",
		"soundId": None,
		"iconId": "test"
	}

	payloads = [
		payload__invalid_sound,
		payload__invalid_icon,
	]
	errors = [
		"""body -> soundId
  value is not a valid integer (type=type_error.integer)""",
		"""body -> iconId
  value is not a valid integer (type=type_error.integer)""",
		"""body -> name
  none is not an allowed value (type=type_error.none.not_allowed)"""
	]

	for (payload, error) in zip(payloads, errors):
		with pytest.raises(RequestValidationError) as exception:
			client.post("/buttons/1", json=payload)
		assert error in str(exception.value)


def test__remove_button__success():
	response_success = client.delete("/buttons/1")
	assert response_success.status_code == 204


def test__remove_button__not_found():
	with pytest.raises(HTTPException) as exception:
		client.delete("/buttons/999")
	assert exception.value.status_code == 404
	assert exception.value.detail == "Button not found"
