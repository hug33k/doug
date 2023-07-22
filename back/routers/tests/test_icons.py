from fastapi.testclient import TestClient
from fastapi.exceptions import RequestValidationError
from fastapi import HTTPException
import pytest
from tests.misc import check_key_in_dict
from ..icons import router


client = TestClient(router)


def assert_icon_read_class(icon):
	assert isinstance(icon, dict)
	check_key_in_dict(icon, "id", int)
	check_key_in_dict(icon, "name", str)
	check_key_in_dict(icon, "path", str)


def test__get_icons__success():
	response_success = client.get("/icons")
	data = response_success.json()
	assert response_success.status_code == 200
	assert isinstance(data, list)
	for item in data:
		assert_icon_read_class(item)


def test__get_icon__success():
	response_success = client.get("/icons/1")
	data = response_success.json()
	assert response_success.status_code == 200
	assert_icon_read_class(data)


def test__get_icon__not_found():
	with pytest.raises(HTTPException) as exception:
		client.get("/icons/999")
	assert exception.value.status_code == 404
	assert exception.value.detail == "Icon not found"


def test__get_icon__invalid_format():
	with pytest.raises(RequestValidationError) as exception:
		client.get("/icons/a")
	assert """path -> icon_id\n  value is not a valid integer (type=type_error.integer)""" in str(exception.value)


def test__add_icon__success():
	payload_default = {
		"name": "Button test",
		"path": "/path"
	}
	payload__non_required_fields = {
		"name": "Button test non required fields",
		"path": "/path",
		"not_a_field": True
	}

	payloads = [
		payload_default,
		payload__non_required_fields
	]

	for payload in payloads:
		response_success = client.post("/icons", json=payload)
		data = response_success.json()
		assert response_success.status_code == 201
		assert_icon_read_class(data)
		assert data["name"] == payload["name"]
		assert data["path"] == payload["path"]
		assert not "not_a_field" in data


def test__add_icon__invalid_format():
	payload__invalid_name = {
		"name": None,
		"path": "/path"
	}
	payload__invalid_path = {
		"name": "Icon",
		"path": None
	}
	payload__no_body = {}

	payloads = [
		payload__invalid_name,
		payload__invalid_path,
		payload__no_body
	]
	errors = [
		"""body -> name
  none is not an allowed value (type=type_error.none.not_allowed)""",
		"""body -> path
  none is not an allowed value (type=type_error.none.not_allowed)""",
		"""body -> name
  field required (type=value_error.missing)"""
	]

	for (payload, error) in zip(payloads, errors):
		with pytest.raises(RequestValidationError) as exception:
			client.post("/icons", json=payload)
		assert error in str(exception.value)


def test__update_icon__success():
	payload_default = {
		"name": "Button test",
		"path": "/path"
	}
	payload__no_path = {
		"name": "Button test no sound",
	}
	payload__no_name = {
		"path": "/path"
	}
	payload__non_required_fields = {
		"name": "Button test non required fields",
		"path": "/path",
		"not_a_field": True
	}
	payload__no_body = {}

	payloads = [
		payload_default,
		payload__no_path,
		payload__no_name,
		payload__non_required_fields,
		payload__no_body
	]

	fields = ["name", "path"]

	for payload in payloads:
		response_success = client.post("/icons/1", json=payload)
		data = response_success.json()
		assert response_success.status_code == 200
		assert_icon_read_class(data)
		for field in fields:
			if field in payload:
				assert data[field] == payload[field]
		assert not "not_a_field" in data


def test__update_icon__not_found():
	payload = {
		"name": "Button test",
		"path": "/path"
	}
	with pytest.raises(HTTPException) as exception:
		client.post("/icons/999", json=payload)
	assert exception.value.status_code == 404
	assert exception.value.detail == "Icon not found"


def test__remove_icon__success():
	response_success = client.delete("/icons/1")
	assert response_success.status_code == 204


def test__remove_icon__not_found():
	with pytest.raises(HTTPException) as exception:
		client.delete("/icons/999")
	assert exception.value.status_code == 404
	assert exception.value.detail == "Icon not found"
