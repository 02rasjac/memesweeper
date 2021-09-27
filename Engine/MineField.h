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
		void Draw(Vei2& gridPos, const bool gameOver, Graphics& gfx);
		void Reveal(const Vei2& gridPos, Tile* tiles);
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
	void Draw(const bool gameOver, Graphics& gfx);
	bool ProcessRevealClick(const Vei2& screenPos);
	void ProcessFlagClick(const Vei2& screenPos);
	bool CheckWin();
	const Vei2& GetGridPos(const Vei2& screenPos);
	Tile& TileAt(const Vei2& gridPos);
	static const Vei2 GetTopLeft();
private:
	static constexpr int width = 10;
	static constexpr int height = 5;
	static constexpr int left = (Graphics::ScreenWidth - MineField::width * SpriteCodex::tileSize) / 2;
	static constexpr int top = (Graphics::ScreenHeight - MineField::height * SpriteCodex::tileSize) / 2;
	int nBombs;
	int nCorrectFlags = 0;
	RectI background = { {left, top}, width * SpriteCodex::tileSize, height * SpriteCodex::tileSize };
	Tile tiles[width * height];
};