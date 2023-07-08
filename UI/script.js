/*
    NAV
*/

const nav_handler = (e) => {
    e.preventDefault();
    if (e.target.id.startsWith("nav_")) {
        const main = document.querySelector("main");
        main.id = e.target.id.replace("nav_", "mode_");
    }
};

const nav_setup = () => {
    const items = document.querySelectorAll("header nav ul li.nav_item");
    [].forEach.call(items, (item) => {
        item.addEventListener("click", nav_handler);
    });
};

/*
	MODAL
*/

const modal_toggle = () => {
	const modal = modal_get();
	modal.classList.toggle("modal_show");
};

const modal_setup = () => {
	const modal = modal_get();
	modal.addEventListener("click", (e) => {
		if (e.target !== e.currentTarget)
			return;
		modal_toggle();
	});
};

const modal_get = () => {
	return document.querySelector("div.modal");
};

const modal_set_title = (title) => {
	const modal = modal_get();
	modal.querySelector("#modal_title").innerText = title;
};

const modal_set_content = (content) => {
	const modal = modal_get();
	modal.querySelector("#modal_content").innerHTML = content;
};

/*
    TRIGGERS
*/

const button_triger = (e, bypassId=null) => {
    console.log(e);
    const targetSplit = e.target.id.split("_");
    var targetId = null;
    if (targetSplit[1] === "button")
        targetId = targetSplit[2];
    else if (bypassId !== null)
        targetId = bypassId;
    if (targetId)
        alert(`Le bouton ${targetId} a été pressé !`);
};

const sound_triger = (e, bypassId=null) => {
    console.log(e);
    const targetSplit = e.target.id.split("_");
    var targetId = null;
    if (targetSplit[1] === "sound")
        targetId = targetSplit[2];
    else if (bypassId !== null)
        targetId = bypassId;
    if (targetId)
        alert(`Le son ${targetId} a été pressé !`);
};

/*
    HOME
*/

const home_show_buttons = (buttons) => {
    const container = document.querySelector("section#section_home ul#home_buttons")
    while (container.firstChild)
        container.removeChild(container.firstChild);
    buttons.forEach((button) => {
        const buttonNode = document.createElement("li");
        buttonNode.id = `home_button_${button.id}`;
        buttonNode.classList.add("button");
        buttonNode.innerText = button.name;
        buttonNode.addEventListener("click", button_triger);
        container.appendChild(buttonNode);
    });
};

const home_show_sounds = (sounds) => {
    const container = document.querySelector("section#section_home ul#home_sounds")
    while (container.firstChild)
        container.removeChild(container.firstChild);
    sounds.forEach((sound) => {
        const soundNode = document.createElement("li");
        soundNode.id = `home_sound_${sound.id}`;
        soundNode.classList.add("button");
        soundNode.innerText = sound.name;
        soundNode.addEventListener("click", sound_triger);
        container.appendChild(soundNode);
    });
};

const home_show_icons = (icons) => {
    const container = document.querySelector("section#section_home ul#home_icons")
    while (container.firstChild)
        container.removeChild(container.firstChild);
    icons.forEach((icon) => {
        const iconNode = document.createElement("li");
        iconNode.id = `home_icon_${icon.id}`;
        iconNode.classList.add("button");
        iconNode.innerText = icon.name;
        container.appendChild(iconNode);
    });
};

/*
    BUTTONS
*/

const buttons_show_buttons = (buttons) => {
    const showData = {
        "Nom": "name",
        "Id": "id",
        "Son": "soundId",
        "Icone": "iconId"
    };
    const container = document.querySelector("section#section_buttons ul#buttons_buttons");
    while (container.firstChild)
        container.removeChild(container.firstChild);
    buttons.forEach((button) => {
        const buttonNode = generate_button_data_node(button, showData, button_delete, button_generate_form, button_triger);
        container.appendChild(buttonNode);
    });
};

const button_generate_form = (id) => {
	return API_button_get(id, (data) => {
		modal_set_title(`Modifier Bouton ${id}`);
		const form = `<form action="">
			<div class="form_field">
				<label for="name">Nom</label>
				<input id="name" name="name" type="text" placeholder="Nom" value="${data.name}"/>
			</div>
			<div class="form_field">
				<label for="soundId">Son</label>
				<select id="soundId" name="soundId"></select>
			</div>
			<div class="form_field">
				<label for="iconId">Icone</label>
				<select id="iconId" name="iconId"></select>
			</div>
			<div class="form_actions">
				<button type="button" class="form_action_cancel" onclick="((e) => {
					e.preventDefault();
					modal_toggle();
				})(event)">Annuler</button>
				<button type="submit" class="form_action_submit">Sauvegarder</button>
			</div>
		</form>`;
		modal_set_content(form);
		const modal = modal_get();
		const formNode = modal.querySelector("form");
		formNode.addEventListener("submit", (e) => {
			e.preventDefault();
			const newData = {};
			const fieldsToNumber = ["soundId", "iconId"];
			Array.from(new FormData(formNode)).forEach(([key, value]) => {
				newData[key] = (fieldsToNumber.indexOf(key) !== -1 ? parseInt(value) : value);
			});
			return API_button_post(id, newData, () => {
				modal_toggle();
				buttons_get();
			});
		});
		
		API_sounds_get((sounds) => {
			const selectNode = formNode.querySelector("select#soundId");
			selectNode.innerHTML = "";
			sounds.forEach((sound) => {
				selectNode.innerHTML += `<option value="${sound.id}" ${data.soundId === sound.id ? "selected" : ""}>${sound.name}</option>`;
			});
		});
		
		API_icons_get((icons) => {
			const selectNode = formNode.querySelector("select#iconId");
			selectNode.innerHTML = "";
			icons.forEach((icon) => {
				selectNode.innerHTML += `<option value="${icon.id}" ${data.iconId === icon.id ? "selected" : ""}>${icon.name}</option>`;
			});
		});
	});
};

/*
    SOUNDS
*/

const sounds_show_sounds = (sounds) => {
    const showData = {
        "Nom": "name",
        "Id": "id",
        "Path": "path"
    };
    const container = document.querySelector("section#section_sounds ul#sounds_sounds")
    while (container.firstChild)
        container.removeChild(container.firstChild);
    sounds.forEach((sound) => {
        const soundNode = generate_button_data_node(sound, showData, sound_delete, sound_generate_form, sound_triger, sound_generate_image_upload);
        container.appendChild(soundNode);
    });
};

const sound_generate_form = (id) => {
	return API_sound_get(id, (data) => {
		modal_set_title(`Modifier Son ${id}`);
		const form = `<form action="">
			<div class="form_field">
				<label for="name">Nom</label>
				<input id="name" name="name" type="text" placeholder="Nom" value="${data.name}"/>
			</div>
			<div class="form_actions">
				<button type="button" class="form_action_cancel" onclick="((e) => {
					e.preventDefault();
					modal_toggle();
				})(event)">Annuler</button>
				<button type="submit" class="form_action_submit">Sauvegarder</button>
			</div>
		</form>`;
		modal_set_content(form);
		const modal = modal_get();
		const formNode = modal.querySelector("form");
		formNode.addEventListener("submit", (e) => {
			e.preventDefault();
			const newData = {};
			Array.from(new FormData(formNode)).forEach(([key, value]) => {
				newData[key] = value;
			});
			return API_sound_post(id, newData, () => {
				modal_toggle();
				sounds_get();
			});
		});
	});
};

const sound_generate_image_upload = (id) => {
	return API_sound_get(id, (data) => {
		modal_set_title(`Modifier Image pour Son ${id}`);
		const form = `<form action="">
			<div class="form_field">
				<label for="sound">Son</label>
				<input id="sound" name="sound" type="file"/>
			</div>
			<div class="form_actions">
				<button type="button" class="form_action_cancel" onclick="((e) => {
					e.preventDefault();
					modal_toggle();
				})(event)">Annuler</button>
				<button type="submit" class="form_action_submit">Envoyer</button>
			</div>
		</form>`;
		modal_set_content(form);
		const modal = modal_get();
		const formNode = modal.querySelector("form");
		formNode.addEventListener("submit", (e) => {
			e.preventDefault();
			console.log(e);

			return API_sound_upload(id, e.target[0].files[0], () => {
				console.log("DONE");
				modal_toggle();
				sounds_get();
			});
		});
	});
};

/*
    ICONS
*/

const icons_show_icons = (icons) => {
    const showData = {
        "Nom": "name",
        "Id": "id",
        "Path": "path"
    };
    const container = document.querySelector("section#section_icons ul#icons_icons")
    while (container.firstChild)
        container.removeChild(container.firstChild);
    icons.forEach((icon) => {
        const iconNode = generate_button_data_node(icon, showData, icon_delete, icon_generate_form, null, icon_generate_image_upload);
        container.appendChild(iconNode);
    });
};

const icon_generate_form = (id) => {
	return API_icon_get(id, (data) => {
		modal_set_title(`Modifier Icone ${id}`);
		const form = `<form action="">
			<div class="form_field">
				<label for="name">Nom</label>
				<input id="name" name="name" type="text" placeholder="Nom" value="${data.name}"/>
			</div>
			<div class="form_actions">
				<button type="button" class="form_action_cancel" onclick="((e) => {
					e.preventDefault();
					modal_toggle();
				})(event)">Annuler</button>
				<button type="submit" class="form_action_submit">Sauvegarder</button>
			</div>
		</form>`;
		modal_set_content(form);
		const modal = modal_get();
		const formNode = modal.querySelector("form");
		formNode.addEventListener("submit", (e) => {
			e.preventDefault();
			const newData = {};
			Array.from(new FormData(formNode)).forEach(([key, value]) => {
				newData[key] = value;
			});
			return API_icon_post(id, newData, () => {
				modal_toggle();
				icons_get();
			});
		});
	});
};

const icon_generate_image_upload = (id) => {
	return API_icon_get(id, (data) => {
		modal_set_title(`Modifier Image pour Icone ${id}`);
		const form = `<form action="">
			<div class="form_field">
				<label for="img">Image</label>
				<input id="img" name="img" type="file"/>
			</div>
			<div class="form_actions">
				<button type="button" class="form_action_cancel" onclick="((e) => {
					e.preventDefault();
					modal_toggle();
				})(event)">Annuler</button>
				<button type="submit" class="form_action_submit">Envoyer</button>
			</div>
		</form>`;
		modal_set_content(form);
		const modal = modal_get();
		const formNode = modal.querySelector("form");
		formNode.addEventListener("submit", (e) => {
			e.preventDefault();
			console.log(e);

			return API_icon_upload(id, e.target[0].files[0], () => {
				console.log("DONE");
				modal_toggle();
				icons_get();
			});
		});
	});
};

/*
    API
*/

const buttons_show = (buttons) => {
    home_show_buttons(buttons);
    buttons_show_buttons(buttons);
};

const buttons_get = () => {
    API_buttons_get(buttons_show);	
};

const button_delete = (id) => {
    API_button_delete(id, buttons_get);
};

const sounds_show = (sounds) => {
    home_show_sounds(sounds);
    sounds_show_sounds(sounds);
};

const sounds_get = () => {
	API_sounds_get(sounds_show);
}

const sound_delete = (id) => {
    API_sound_delete(id, sounds_get);
};

const icons_show = (icons) => {
    home_show_icons(icons);
    icons_show_icons(icons);
};

const icons_get = () => {
	API_icons_get(icons_show);
}

const icon_delete = (id) => {
    API_icon_delete(id, icons_get);
};

/*
    MISC
*/

const generate_button_data_node = (item, showData, deleteTrigger, formTrigger, testTrigger=null, editImageTrigger=null) => {
    const buttonNode = document.createElement("li");
    buttonNode.id = `buttons_button_${item.id}`;
    buttonNode.classList.add("buttondata");
    buttonNode.innerHTML = `
        <article>
            <div class="buttondata_data">
                ${Object.keys(showData).map((key) => {
					var out = "";
					out += `<div>${key} : ${item[showData[key]]}</div>`;
					if (key === "Path")
						out += `<img src="http://localhost:8000${item[showData[key]]}"/>`;
					return out;
				}).join("\n")}
            </div>
            <div class="buttondata_actions">
                ${testTrigger ? `<button type="button" class="buttondata_action_test" onclick="${testTrigger.name}(event, ${item.id})">Tester</button>` : ""}
                ${editImageTrigger ? `<button type="button" class="buttondata_action_image" onclick="((e) => {
					e.preventDefault();
					${editImageTrigger.name}(${item.id});
					modal_toggle();
				})(event)">Modifier l'image</button>` : ""}
                <button type="button" class="buttondata_action_edit" onclick="((e) => {
					e.preventDefault();
					${formTrigger.name}(${item.id});
					modal_toggle();
				})(event)">Modifier</button>
                <button type="button" class="buttondata_action_delete" onclick="((e) => {
                    e.preventDefault();
                    const response = confirm('Voulez vous vraiment supprimer cet élément ?');
                    if (response)
                        ${deleteTrigger.name}(${item.id});
                    return false;
                })(event)">Supprimer</button>
            </div>
        </article>`;
    return buttonNode;
};

window.onload = () => {
    nav_setup();
	modal_setup();
    API_buttons_get(buttons_show);
    API_sounds_get(sounds_show);
    API_icons_get(icons_show);
};
