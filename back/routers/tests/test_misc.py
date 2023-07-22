from fastapi.testclient import TestClient
from tests.misc import check_key_in_dict
from ..misc import router


client = TestClient(router)


def test_get_status():
	response = client.get("/status")
	data = response.json()
	assert response.status_code == 200
	assert isinstance(data, dict)
	check_key_in_dict(data, "status", bool)
	assert data["status"]
	check_key_in_dict(data, "date", str)
