#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>

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
	//get pointer to camera for convenience:
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();

	// initialize game state
	// pizzas and cakes
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
			pizzas[3].type = drawable.pipeline.type;
			pizzas[3].start = drawable.pipeline.start;
			pizzas[3].count = drawable.pipeline.count;
			pizzas[9].type = drawable.pipeline.type;
			pizzas[9].start = drawable.pipeline.start;
			pizzas[9].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Cake0101") {
			cakes[0].type = drawable.pipeline.type;
			cakes[0].start = drawable.pipeline.start;
			cakes[0].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Cake0110") {
			cakes[1].type = drawable.pipeline.type;
			cakes[1].start = drawable.pipeline.start;
			cakes[1].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Cake0111") {
			cakes[2].type = drawable.pipeline.type;
			cakes[2].start = drawable.pipeline.start;
			cakes[2].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Cake1001") {
			cakes[3].type = drawable.pipeline.type;
			cakes[3].start = drawable.pipeline.start;
			cakes[3].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Cake1010") {
			cakes[4].type = drawable.pipeline.type;
			cakes[4].start = drawable.pipeline.start;
			cakes[4].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Cake1011") {
			cakes[5].type = drawable.pipeline.type;
			cakes[5].start = drawable.pipeline.start;
			cakes[5].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Cake1101") {
			cakes[7].type = drawable.pipeline.type;
			cakes[7].start = drawable.pipeline.start;
			cakes[7].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Cake1110") {
			cakes[8].type = drawable.pipeline.type;
			cakes[8].start = drawable.pipeline.start;
			cakes[8].count = drawable.pipeline.count;
		} else if (drawable.transform->name == "Cake1111") {
			cakes[6].type = drawable.pipeline.type;
			cakes[6].start = drawable.pipeline.start;
			cakes[6].count = drawable.pipeline.count;
			cakes[9].type = drawable.pipeline.type;
			cakes[9].start = drawable.pipeline.start;
			cakes[9].count = drawable.pipeline.count;
		} 
	}

	// cursor
	scene.drawables.clear();
	highlight_transform = new Scene::Transform;
	highlight_transform->name = "Highlight";
	highlight_transform->position = glm::vec3(0.0f, 0.0f, 0.0f);
	highlight_transform->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	scene.drawables.emplace_back(highlight_transform);

	// board
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

			Scene::Drawable &d = scene.drawables.back();
			d.pipeline = lit_color_texture_program_pipeline;
			d.pipeline.vao = board_meshes_for_lit_color_texture_program;
			d.pipeline.type = chunk_pipeline.type;
			d.pipeline.start = chunk_pipeline.start;
			d.pipeline.count = chunk_pipeline.count;

			board[r*SIZE+c] = false;
		}
	}
	highlight_transform->parent = chunk_transforms[cur_row*SIZE+cur_col];
	
	// physics from drop animation
	for (uint16_t idx = 0; idx < SIZE*SIZE; idx++) {
		piece_drop_speed[idx] = -mass;
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
			cur_row = (uint16_t) std::max(0, cur_row - 1);
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			cur_row = (uint16_t) std::min(num_rows - 1, cur_row + 1);
			return true;
		} else if (evt.key.keysym.sym == SDLK_LEFT) {
			cur_col = (uint16_t) std::max(0, cur_col - 1);
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			cur_col = (uint16_t) std::min(num_cols - 1, cur_col + 1);
			return true;
		} else if (evt.key.keysym.sym == SDLK_RETURN) {
			if (gameState == GameState::IN_PROGRESS) {
				if (drop_piece_on_board()) {
					generate_new_piece(false);
				}
			}
			return true;
		} else if (evt.key.keysym.sym == SDLK_x) {
			if (gameState == GameState::IN_PROGRESS) {
				generate_new_piece(true);
			}
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

std::list<std::string> PlayMode::get_neighbor_names() {
	std::list<std::string> neighbors;
	if (cur_row - 1 >= 0 && board[(cur_row-1)*SIZE+cur_col]) {
		neighbors.push_back(board_piece_transforms[(cur_row-1)*SIZE+cur_col]->name);
	}
	if (cur_row + 1 < SIZE && board[(cur_row+1)*SIZE+cur_col]) {
		neighbors.push_back(board_piece_transforms[(cur_row+1)*SIZE+cur_col]->name);
	}
	if (cur_col - 1 >= 0 && board[cur_row*SIZE+(cur_col-1)]) {
		neighbors.push_back(board_piece_transforms[cur_row*SIZE+(cur_col-1)]->name);
	}
	if (cur_col + 1 < SIZE && board[cur_row*SIZE+(cur_col+1)]) {
		neighbors.push_back(board_piece_transforms[cur_row*SIZE+(cur_col+1)]->name);
	}
	return neighbors;
}

int PlayMode::get_index_from_name(std::string name) {
	if (name[0] == 'P') {
		return std::stoi(name.substr(6));
	} else {
		return std::stoi(name.substr(5));
	}
}

bool PlayMode::drop_piece_on_board() {
	if (board[cur_row*SIZE+cur_col]) {
		if (DEBUG) {
			std::cout << "[logger] (" << cur_row << "," << cur_col << ") occupied" << std::endl;
		}
		msg_flag = 2;
		return false; // current position already occupied
	}

	// check if placement is valid
	std::list<std::string> neighbors = get_neighbor_names();
	if (neighbors.size() > 0) {
		std::string cur_name = cur_piece->name;
		bool flag = false;
		for (auto const &name : neighbors) {
			int cur_idx = get_index_from_name(cur_name);
			int idx = get_index_from_name(name);
			int count = 0;
			for (int i = 0; i < 4; i++) {
				if ((idx & 1) == 1 && (cur_idx & 1) == 1) {
					count += 1;
				}
				idx >>= 1;
				cur_idx >>= 1;
			}
			// share at least 2 toppings with a piece of the same kind among orthogonal neighbors
			if (name[0] == cur_name[0] && count >= 2) {
				flag = true;
				break;
			}
		}
		if (!flag) {
			msg_flag = 1;
			return false;
		}
	}

	// place the piece and set its drop animation
	board[cur_row*SIZE+cur_col] = true;
	board_piece_transforms[cur_row*SIZE+cur_col] = cur_piece;
	cur_piece->position = glm::vec3(-0.8f, 0.0f, 20.0f);
	cur_piece->scale = glm::vec3(1.0f, 1.0f, 5.0f);
	if (cur_piece->name[0] == 'C') {
		cur_piece->scale = glm::vec3(1.2f, 1.2f, 3.0f);
	}
	cur_piece->parent = chunk_transforms[cur_row*SIZE+cur_col];
	pieces_dropped += 1;
	msg_flag = 0;
	update_score();

	if (pieces_dropped == SIZE * SIZE) {
		gameState = GameState::END;
		msg_flag = 4;
	}
	return true;
}

void PlayMode::generate_new_piece(bool discard) {
	if (discard) {
		scene.drawables.pop_back();
	}

	if (pieces_remaining == 0) {
		gameState = GameState::END;
		msg_flag = 3;
		if (DEBUG) {
			std::cout << "[logger] no more pieces remaining" << std::endl;
		}	
		return;
	} 

	bool is_pizza = rand() % 2 == 0;
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
		int idx = rand() % 10;
		cur_piece = new Scene::Transform;
		cur_piece->name = "Cake " + std::to_string(cake_idx_to_flag[idx]);
		cur_piece->position = glm::vec3(1.0f, 0.6f, -5.0f);
		cur_piece->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		cur_piece->scale = glm::vec3(0.8f, 0.8f, 1.0f);
		cur_piece->parent = camera->transform;
		scene.drawables.emplace_back(cur_piece);

		Scene::Drawable &drawable = scene.drawables.back();
		drawable.pipeline = lit_color_texture_program_pipeline;
		drawable.pipeline.vao = board_meshes_for_lit_color_texture_program;
		drawable.pipeline.type = cakes[idx].type;
		drawable.pipeline.start = cakes[idx].start;
		drawable.pipeline.count = cakes[idx].count;
	}
	pieces_remaining -= 1;
}

void PlayMode::update_score() {
	std::list<std::string> neighbors = get_neighbor_names();
	if (neighbors.size() > 0) {
		std::string cur_name = cur_piece->name;
		for (auto const &name : neighbors) {
			int cur_idx = get_index_from_name(cur_name);
			int idx = get_index_from_name(name);
			for (int i = 0; i < 4; i++) {
				if ((idx & 1) == 1 && (cur_idx & 1) == 1) {
					cur_score += 10;
				}
				idx >>= 1;
				cur_idx >>= 1;
			}
		}
	}

	if (DEBUG) {
		std::cout << "[progress] score: " << cur_score << "|occupied: " << pieces_dropped << "|remaining: " << pieces_remaining << std::endl;
	}	
	return;
}

void PlayMode::update(float elapsed) {
	highlight_transform->parent = chunk_transforms[cur_row*SIZE+cur_col];

	{ // drop the pieces
		for (uint16_t idx = 0; idx < SIZE*SIZE; idx++) {
			if (!board[idx] || momentum[idx] <= 0.0f) {
				continue;
			}
			float offset;
			if (board_piece_transforms[idx]->name[0] == 'P') {
				offset = board_z_offset_pizza;
			} else {
				offset = board_z_offset_cake;
			}
			piece_drop_speed[idx] += gravity * elapsed;
			board_piece_transforms[idx]->position.z = board_piece_transforms[idx]->position.z + elapsed * piece_drop_speed[idx];
			if (board_piece_transforms[idx]->position.z <= offset) {
				board_piece_transforms[idx]->position.z = offset;
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
		lines.draw_text("Mouse & WASD - move camera | ESC - ungrab mouse | Arrow keys - select | RETURN - place | X - skip",
			glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		float ofs = 2.0f / drawable_size.y;
		lines.draw_text("Mouse & WASD - move camera | ESC - ungrab mouse | Arrow keys - select | RETURN - place | X - skip",
			glm::vec3(-aspect + 0.1f * H + ofs, -1.0 + + 0.1f * H + ofs, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));

		if (gameState == GameState::END) {
			if (msg_flag == 3) {
				lines.draw_text("No more food remaining! Game ended with score " + std::to_string(cur_score),
					glm::vec3(-aspect + 0.1f * H, 0.9f, 0.0),
					glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
					glm::u8vec4(0x00, 0x00, 0x00, 0x00));
				float ofs = 2.0f / drawable_size.y;
				lines.draw_text("No more food remaining! Game ended with score " + std::to_string(cur_score),
					glm::vec3(-aspect + 0.1f * H + ofs, 0.9f + ofs, 0.0),
					glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
					glm::u8vec4(0xff, 0xff, 0xff, 0x00));
			} else if (msg_flag == 4) {
				lines.draw_text("You have filled all the space! Game completed with score " + std::to_string(cur_score),
					glm::vec3(-aspect + 0.1f * H, 0.9f, 0.0),
					glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
					glm::u8vec4(0x00, 0x00, 0x00, 0x00));
				float ofs = 2.0f / drawable_size.y;
				lines.draw_text("You have filled all the space! Game completed with score " + std::to_string(cur_score),
					glm::vec3(-aspect + 0.1f * H + ofs, 0.9f + ofs, 0.0),
					glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
					glm::u8vec4(0xff, 0xff, 0xff, 0x00));
			}
		} else {
			lines.draw_text("Current Score - " + std::to_string(cur_score) + " | # of Pieces Remaining - " + std::to_string(pieces_remaining) + "/" + std::to_string(50),
				glm::vec3(-aspect + 0.1f * H, 0.9f, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				glm::u8vec4(0x00, 0x00, 0x00, 0x00));
			float ofs = 2.0f / drawable_size.y;
			lines.draw_text("Current Score - " + std::to_string(cur_score) + " | # of Pieces Remaining - " + std::to_string(pieces_remaining) + "/" + std::to_string(50),
				glm::vec3(-aspect + 0.1f * H + ofs, 0.9f + ofs, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				glm::u8vec4(0xff, 0xff, 0xff, 0x00));
			if (msg_flag == 1) {
				lines.draw_text("Must share at least 2 toppings with any adjacent (orthogonal) piece or be isolated",
					glm::vec3(-aspect + 0.1f * H, 0.8f, 0.0),
					glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
					glm::u8vec4(0x00, 0x00, 0x00, 0x00));
				float ofs = 2.0f / drawable_size.y;
				lines.draw_text("Must share at least 2 toppings with any adjacent (orthogonal) piece or be isolated",
					glm::vec3(-aspect + 0.1f * H + ofs, 0.8f + ofs, 0.0),
					glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
					glm::u8vec4(0xff, 0xff, 0xff, 0x00));
			} else if (msg_flag == 2) {
				lines.draw_text("Space already filled",
					glm::vec3(-aspect + 0.1f * H, 0.8f, 0.0),
					glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
					glm::u8vec4(0x00, 0x00, 0x00, 0x00));
				float ofs = 2.0f / drawable_size.y;
				lines.draw_text("Space already filled",
					glm::vec3(-aspect + 0.1f * H + ofs, 0.8f + ofs, 0.0),
					glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
					glm::u8vec4(0xff, 0xff, 0xff, 0x00));
			}
		}
	}
}
