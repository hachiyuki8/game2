#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

// for glm::to_stirng();
#include <glm/gtx/string_cast.hpp>

#include <random>

const std::string CAKE = "Cake";
const std::string CHOCOLATE = "Chocolate";
const std::string KIWI = "Kiwi";

const std::string CHUNK = "Chunk";
const std::string HIGHLIGHT = "Highlight";

GLuint board_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > board_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("pizza.pnct"));
	board_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > board_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("pizza.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = board_meshes->lookup(mesh_name);
		if (DEBUG) {
			std::cout << transform->name << " " << mesh_name << std::endl;
		}

		scene.drawables.emplace_back(transform);
		Scene::Drawable &drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;

		drawable.pipeline.vao = board_meshes_for_lit_color_texture_program;
		drawable.pipeline.type = mesh.type;
		drawable.pipeline.start = mesh.start;
		drawable.pipeline.count = mesh.count;
	});
});

PlayMode::PlayMode() : scene(*board_scene) {
	//get pointers to leg for convenience:
	// for (auto &transform : scene.transforms) {
	// 	if (transform.name == "Hip.FL") hip = &transform;
	// 	else if (transform.name == "UpperLeg.FL") upper_leg = &transform;
	// 	else if (transform.name == "LowerLeg.FL") lower_leg = &transform;
	// }
	// if (hip == nullptr) throw std::runtime_error("Hip not found.");
	// if (upper_leg == nullptr) throw std::runtime_error("Upper leg not found.");
	// if (lower_leg == nullptr) throw std::runtime_error("Lower leg not found.");

	// hip_base_rotation = hip->rotation;
	// upper_leg_base_rotation = upper_leg->rotation;
	// lower_leg_base_rotation = lower_leg->rotation;

	//get pointer to camera for convenience:
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();

	// initialize game state
	for (auto &drawable : scene.drawables) {
		if (drawable.transform->name == CHUNK) {
			chunk_pipeline.type = drawable.pipeline.type;
			chunk_pipeline.start = drawable.pipeline.start;
			chunk_pipeline.count = drawable.pipeline.count;
		} else if (drawable.transform->name == HIGHLIGHT) {
			highlight_pipeline.type = drawable.pipeline.type;
			highlight_pipeline.start = drawable.pipeline.start;
			highlight_pipeline.count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Pizza0011") {
			pizzas[0].type = drawable.pipeline.type;
			pizzas[0].start = drawable.pipeline.start;
			pizzas[0].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Pizza0101") {
			pizzas[1].type = drawable.pipeline.type;
			pizzas[1].start = drawable.pipeline.start;
			pizzas[1].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Pizza0110") {
			pizzas[2].type = drawable.pipeline.type;
			pizzas[2].start = drawable.pipeline.start;
			pizzas[2].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Pizza1001") {
			pizzas[3].type = drawable.pipeline.type;
			pizzas[3].start = drawable.pipeline.start;
			pizzas[3].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Pizza1010") {
			pizzas[4].type = drawable.pipeline.type;
			pizzas[4].start = drawable.pipeline.start;
			pizzas[4].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Pizza0111") {
			pizzas[5].type = drawable.pipeline.type;
			pizzas[5].start = drawable.pipeline.start;
			pizzas[5].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Pizza1011") {
			pizzas[6].type = drawable.pipeline.type;
			pizzas[6].start = drawable.pipeline.start;
			pizzas[6].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Pizza1101") {
			pizzas[7].type = drawable.pipeline.type;
			pizzas[7].start = drawable.pipeline.start;
			pizzas[7].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Pizza1110") {
			pizzas[8].type = drawable.pipeline.type;
			pizzas[8].start = drawable.pipeline.start;
			pizzas[8].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Pizza1111") {
			pizzas[9].type = drawable.pipeline.type;
			pizzas[9].start = drawable.pipeline.start;
			pizzas[9].count = drawable.pipeline.count;
		} else if (drawable.transform->name == CAKE) {
			cake_pipeline.type = drawable.pipeline.type;
			cake_pipeline.start = drawable.pipeline.start;
			cake_pipeline.count = drawable.pipeline.count;
		} 
	}

	scene.drawables.clear();
	highlight_transform = new Scene::Transform;
	highlight_transform->name = "Highlight";
	highlight_transform->position = glm::vec3(0.0f, 0.0f, 0.0f);
	highlight_transform->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	scene.drawables.emplace_back(highlight_transform);

	Scene::Drawable &drawable = scene.drawables.back();
	drawable.pipeline = lit_color_texture_program_pipeline;
	drawable.pipeline.vao = board_meshes_for_lit_color_texture_program;
	drawable.pipeline.type = highlight_pipeline.type;
	drawable.pipeline.start = highlight_pipeline.start;
	drawable.pipeline.count = highlight_pipeline.count;
	for (uint16_t r = 0; r < num_rows; r++) {
		for (uint16_t c = 0; c < num_cols; c++) {
			chunk_transforms[r*SIZE+c] = new Scene::Transform;
			chunk_transforms[r*SIZE+c]->name = "Chunk " + std::to_string(r*SIZE+c);
			chunk_transforms[r*SIZE+c]->position = glm::vec3(-chunk_size*r, -chunk_size*c, -1.0f);
			chunk_transforms[r*SIZE+c]->scale = glm::vec3(1.0f, 1.0f, 0.2f);
			scene.drawables.emplace_back(chunk_transforms[r*SIZE+c]);

			Scene::Drawable &drawable = scene.drawables.back();
			drawable.pipeline = lit_color_texture_program_pipeline;
			drawable.pipeline.vao = board_meshes_for_lit_color_texture_program;
			drawable.pipeline.type = chunk_pipeline.type;
			drawable.pipeline.start = chunk_pipeline.start;
			drawable.pipeline.count = chunk_pipeline.count;

			board[r*SIZE+c] = false;
		}
	}
	highlight_transform->parent = chunk_transforms[cur_row*SIZE+cur_col];
	
	for (uint16_t idx = 0; idx < SIZE*SIZE; idx++) {
		piece_drop_speed[idx] = -1.0f;
		momentum[idx] = 1.0f;
	}

	srand(time(NULL));
	generate_new_piece(false);
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		} else if (evt.key.keysym.sym == SDLK_a) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.downs += 1;
			down.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			cur_row = std::max(0, cur_row - 1);
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			cur_row = std::min(num_rows - 1, cur_row + 1);
			return true;
		} else if (evt.key.keysym.sym == SDLK_LEFT) {
			cur_col = std::max(0, cur_col - 1);
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			cur_col = std::min(num_cols - 1, cur_col + 1);
			return true;
		} else if (evt.key.keysym.sym == SDLK_RETURN) {
			if (drop_piece_on_board()) {
				generate_new_piece(false);
			}
			return true;
		} else if (evt.key.keysym.sym == SDLK_x) {
			generate_new_piece(true);
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.pressed = false;
			return true;
		}
	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
			return true;
		}
	} else if (evt.type == SDL_MOUSEMOTION) {
		if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
			glm::vec2 motion = glm::vec2(
				evt.motion.xrel / float(window_size.y),
				-evt.motion.yrel / float(window_size.y)
			);
			camera->transform->rotation = glm::normalize(
				camera->transform->rotation
				* glm::angleAxis(-motion.x * camera->fovy, glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::angleAxis(motion.y * camera->fovy, glm::vec3(1.0f, 0.0f, 0.0f))
			);
			return true;
		}
	}

	return false;
}

bool PlayMode::drop_piece_on_board() {
	if (board[cur_row*SIZE+cur_col]) {
		if (DEBUG) {
			std::cout << "[logger] (" << cur_row << "," << cur_col << ") occupied" << std::endl;
		}
		return false; // current position already occupied
	}

	// TODO: check constraints
	board[cur_row*SIZE+cur_col] = true;
	board_piece_transforms[cur_row*SIZE+cur_col] = cur_piece;

	cur_piece->position = glm::vec3(-0.8f, 0.0f, 10.0f);
	cur_piece->scale = glm::vec3(1.0f, 1.0f, 3.0f);
	cur_piece->parent = chunk_transforms[cur_row*SIZE+cur_col];
	pieces_dropped += 1;
	update_score();
	return true;
}

void PlayMode::generate_new_piece(bool discard) {
	if (discard) {
		scene.drawables.pop_back();
	}

	if (pieces_remaining == 0) {
		gameState = GameState::END;
		if (DEBUG) {
			std::cout << "[logger] no more pieces remaining" << std::endl;
		}	
		return;
	} 

	bool is_pizza = rand() % 2 == 0;
	is_pizza = true;
	if (is_pizza) {
		int idx = rand() % 10;
		cur_piece = new Scene::Transform;
		cur_piece->name = "Pizza " + std::to_string(pizza_idx_to_flag[idx]);
		cur_piece->position = glm::vec3(1.0f, 0.6f, -5.0f);
		cur_piece->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		cur_piece->scale = glm::vec3(0.7f, 0.7f, 1.0f);
		cur_piece->parent = camera->transform;
		scene.drawables.emplace_back(cur_piece);

		Scene::Drawable &drawable = scene.drawables.back();
		drawable.pipeline = lit_color_texture_program_pipeline;
		drawable.pipeline.vao = board_meshes_for_lit_color_texture_program;
		drawable.pipeline.type = pizzas[idx].type;
		drawable.pipeline.start = pizzas[idx].start;
		drawable.pipeline.count = pizzas[idx].count;
	} else {
		std::cout << "DEBUG" << std::endl;
	}
	pieces_remaining -= 1;
}

void PlayMode::update_score() {
	// TODO: check patterns
	if (DEBUG) {
		std::cout << "[progress] score: " << cur_score << "|occupied: " << pieces_dropped << "|remaining: " << pieces_remaining << std::endl;
	}	
	return;
}

void PlayMode::update(float elapsed) {

	//slowly rotates through [0,1):
	// wobble += elapsed / 10.0f;
	// wobble -= std::floor(wobble);

	// hip->rotation = hip_base_rotation * glm::angleAxis(
	// 	glm::radians(5.0f * std::sin(wobble * 2.0f * float(M_PI))),
	// 	glm::vec3(0.0f, 1.0f, 0.0f)
	// );
	// upper_leg->rotation = upper_leg_base_rotation * glm::angleAxis(
	// 	glm::radians(7.0f * std::sin(wobble * 2.0f * 2.0f * float(M_PI))),
	// 	glm::vec3(0.0f, 0.0f, 1.0f)
	// );
	// lower_leg->rotation = lower_leg_base_rotation * glm::angleAxis(
	// 	glm::radians(10.0f * std::sin(wobble * 3.0f * 2.0f * float(M_PI))),
	// 	glm::vec3(0.0f, 0.0f, 1.0f)
	// );

	highlight_transform->parent = chunk_transforms[cur_row*SIZE+cur_col];

	{ // drop the pieces
		for (uint16_t idx = 0; idx < SIZE*SIZE; idx++) {
			if (!board[idx] or momentum[idx] <= 0.0f) {
				continue;
			}
			piece_drop_speed[idx] += gravity * elapsed;
			board_piece_transforms[idx]->position.z = board_piece_transforms[idx]->position.z + elapsed * piece_drop_speed[idx];
			if (board_piece_transforms[idx]->position.z <= board_z_offset) {
				board_piece_transforms[idx]->position.z = board_z_offset;
				piece_drop_speed[idx] = piece_drop_speed[idx] * -0.5f;
				momentum[idx] = piece_drop_speed[idx] * mass;
			}
		}
	}

	{ // move camera

		//combine inputs into a move:
		constexpr float PlayerSpeed = 30.0f;
		glm::vec2 move = glm::vec2(0.0f);
		if (left.pressed && !right.pressed) move.x =-1.0f;
		if (!left.pressed && right.pressed) move.x = 1.0f;
		if (down.pressed && !up.pressed) move.y =-1.0f;
		if (!down.pressed && up.pressed) move.y = 1.0f;

		//make it so that moving diagonally doesn't go faster:
		if (move != glm::vec2(0.0f)) move = glm::normalize(move) * PlayerSpeed * elapsed;

		glm::mat4x3 frame = camera->transform->make_local_to_parent();
		glm::vec3 frame_right = frame[0];
		//glm::vec3 up = frame[1];
		glm::vec3 frame_forward = -frame[2];

		camera->transform->position += move.x * frame_right + move.y * frame_forward;
	}

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//update camera aspect ratio for drawable:
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	GL_ERRORS(); //print any errors produced by this setup code

	// cake->transform->parent = chunk->transform;
	// cake->transform->position = glm::vec3(2.0f, 2.0f, 2.0f);
	// std::cout << glm::to_string(cake->transform->position) << std::endl;
	// std::cout << glm::to_string(chunk->transform->position) << std::endl;
	// for (auto const &drawable : scene.drawables) {
	// 	std::cout << drawable.transform->name << " " << glm::to_string(drawable.transform->position) << std::endl;
	// }
	scene.draw(*camera);

	{ //use DrawLines to overlay some text:
		glDisable(GL_DEPTH_TEST);
		float aspect = float(drawable_size.x) / float(drawable_size.y);
		DrawLines lines(glm::mat4(
			1.0f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		));

		constexpr float H = 0.09f;
		lines.draw_text("Mouse motion rotates camera; WASD moves; escape ungrabs mouse",
			glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		float ofs = 2.0f / drawable_size.y;
		lines.draw_text("Mouse motion rotates camera; WASD moves; escape ungrabs mouse",
			glm::vec3(-aspect + 0.1f * H + ofs, -1.0 + + 0.1f * H + ofs, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
	}
}
