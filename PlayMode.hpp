#include "Mode.hpp"

#include "Scene.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <map>

const bool DEBUG = false;

struct Pizza {
	bool olive;
	bool pepper;
	bool pepperoni;
	bool sausage;
};

struct Cake {
	bool chocolate;
	bool kiwi;
	bool strawberry;
	bool marshmellow;
};

struct Piece {
	bool isPizza = true;
	union {
		struct Pizza pizza;
		struct Cake cake;
	} content;
};

struct PipelineInfo {
	GLenum type = GL_TRIANGLES;
	GLuint start = 0;
	GLuint count = 0; 
};

const uint16_t SIZE = 6;

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;

	PipelineInfo chunk_pipeline;
	PipelineInfo highlight_pipeline;
	PipelineInfo pizzas [10];
	std::map<int, int> pizza_idx_to_flag = {
		{0, 3}, {1, 5}, {2, 6}, {3, 15}, {4, 10}, {5, 7}, {6, 11}, {7, 13}, {8, 14}, {9, 15}
	};
	PipelineInfo cakes [10];
	std::map<int, int> cake_idx_to_flag = {
		{0, 5}, {1, 6}, {2, 7}, {3, 9}, {4, 10}, {5, 11}, {6, 15}, {7, 13}, {8, 14}, {9, 15}
	};

	Scene::Transform *chunk_transforms[SIZE*SIZE];
	Scene::Transform *highlight_transform;

	enum GameState : uint16_t {
		START,
		IN_PROGRESS,
		END,
	};
	GameState gameState = GameState::IN_PROGRESS;

	//board
	uint16_t num_rows = SIZE;
	uint16_t num_cols = SIZE;
	uint16_t cur_row = 0;
	uint16_t cur_col = 0;
	float chunk_size = 2.0f;

	bool board[SIZE*SIZE];
	Scene::Transform *board_piece_transforms[SIZE*SIZE];
	float piece_drop_speed[SIZE*SIZE];
	float momentum[SIZE*SIZE];
	float gravity = -78.4f;
	float mass = 10.0f;
	float board_z_offset_pizza = 1.5f;
	float board_z_offset_cake = 3.0f;

	Scene::Transform *cur_piece;
	uint16_t pieces_dropped = 0;
	uint16_t pieces_remaining = 50;
	uint16_t cur_score = 0;
	// drop the current piece on the selected position on the board if it's valid
	virtual bool drop_piece_on_board();
	virtual void update_score();
	// generate a new piece of pizza or cake if there's any chance left
	virtual void generate_new_piece(bool discard);
	virtual int get_index_from_name(std::string name);
	virtual std::list<std::string> get_neighbor_names();

	int msg_flag = 0;

	//hexapod leg to wobble:
	// Scene::Transform *hip = nullptr;
	// Scene::Transform *upper_leg = nullptr;
	// Scene::Transform *lower_leg = nullptr;
	// glm::quat hip_base_rotation;
	// glm::quat upper_leg_base_rotation;
	// glm::quat lower_leg_base_rotation;
	// float wobble = 0.0f;
	
	//camera:
	Scene::Camera *camera = nullptr;

};
