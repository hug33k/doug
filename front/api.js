/*
	GLOBAL
*/

const _API_base_url = "http://127.0.0.1:8000";

const _API_request = async (endpoint, method, data=null) => {
    let response = await fetch(`${_API_base_url}${endpoint}`, { 
        method,
        body: data ? JSON.stringify(data) : undefined,
		headers: {
			"Content-Type": "application/json"
		}
    });
    return method === "DELETE" ? true : response.json();
};

const _API_file_upload = async (endpoint, file=null) => {
	const formData = new FormData();
	formData.append("file", file);
    let response = await fetch(`${_API_base_url}${endpoint}`, { 
        method: "POST",
        body: formData,
    });
    return response.json();
};

const _API_request_get_all = (resourceType, callback=null) => {
    return _API_request(`/${resourceType}`, "GET")
		.then((data) => callback ? callback(data) : data);
};

const _API_request_get = (resourceType, id, callback=null) => {
    return _API_request(`/${resourceType}/${id}`, "GET")
		.then((data) => callback ? callback(data) : data);
};

const _API_request_post = (resourceType, id, body, callback=null) => {
    return _API_request(`/${resourceType}${id === null ? '' : `/${id}`}`, "POST", body)
		.then((data) => callback ? callback(data) : data);
};

const _API_request_delete = (resourceType, id, callback=null) => {
    return _API_request(`/${resourceType}/${id}`, "DELETE")
            .then((data) => callback ? callback(data) : data);
};

const _API_request_upload = (resourceType, id, file, callback=null) => {
	return _API_file_upload(`/${resourceType}/${id}/upload`, file)
			.then((data) => callback ? callback(data) : data);
};

/*
	BUTTONS
*/

const API_buttons_get = (callback=null) => {
    return _API_request_get_all("buttons", callback);
};

const API_button_get = (id, callback=null) => {
	return _API_request_get("buttons", id, callback);
};

const API_button_post = (id, body, callback=null) => {
	return _API_request_post("buttons", id, body, callback);
};

const API_button_delete = (id, callback=null) => {
	return _API_request_delete("buttons", id, callback);
};

/*
	SOUNDS
*/

const API_sounds_get = (callback=null) => {
    return _API_request_get_all("sounds", callback);
};

const API_sound_get = (id, callback=null) => {
	return _API_request_get("sounds", id, callback);
};

const API_sound_post = (id, body, callback=null) => {
	return _API_request_post("sounds", id, body, callback);
};

const API_sound_delete = (id, callback=null) => {
	return _API_request_delete("sounds", id, callback);
};

const API_sound_upload = (id, file, callback=null) => {
	return _API_request_upload("sounds", id, file, callback);
};

/*
	ICONS
*/

const API_icons_get = (callback=null) => {
    return _API_request_get_all("icons", callback);
};

const API_icon_get = (id, callback=null) => {
	return _API_request_get("icons", id, callback);
};

const API_icon_post = (id, body, callback=null) => {
	return _API_request_post("icons", id, body, callback);
};

const API_icon_delete = (id, callback=null) => {
	return _API_request_delete("icons", id, callback);
};

const API_icon_upload = (id, file, callback=null) => {
	return _API_request_upload("icons", id, file, callback);
};
