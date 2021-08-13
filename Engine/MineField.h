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
		void ToggleFlag();
		const State GetState() const { return state; };
		void CalculateNeighbors(const Vei2& gridPos, const Tile tiles[]);
	private:
		State state = State::hidden;
		bool isBomb = false;
		short nNeighborsBombs = -1;
	};
public:
	MineField(int nBombs);
	void Draw(Graphics& gfx);
	void ProcessLMB(const Vei2& screenPos);
	void ProcessRMB(const Vei2& screenPos);
	Tile& TileAt(const Vei2& gridPos);
private:
	static constexpr int width = 20;
	static constexpr int height = 16;
	RectI background = { {0,0}, width * SpriteCodex::tileSize, height * SpriteCodex::tileSize };
	Tile tiles[width * height];
};