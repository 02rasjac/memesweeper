#pragma once
#include "Graphics.h"
#include "SpriteCodex.h"
#include "RectI.h"
#include "Vei2.h"
class MineField
{
private:
	class Tile {
	public:
		enum class State
		{
			hidden,
			reveald,
			flagged
		};
	public:
		void SpawnBomb();
		bool HasBomb() const { return isBomb; };
		void Draw(Vei2& gridPos, Graphics& gfx);
		void Reveal();
		const State GetState() const { return state; };
	private:
		State state = State::hidden;
		bool isBomb = false;
	};
public:
	MineField(int nBombs);
	void Draw(Graphics& gfx);
private:
	static constexpr int width = 20;
	static constexpr int height = 16;
	RectI background = { {0,0}, width * SpriteCodex::tileSize, height * SpriteCodex::tileSize };
	Tile tiles[width * height];
};