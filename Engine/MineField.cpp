#include <random>
#include <assert.h>
#include "SpriteCodex.h"
#include "MineField.h"

void MineField::Tile::SpawnBomb()
{
	assert(!isBomb);
	isBomb = true;
}

void MineField::Tile::Draw(Vei2& gridPos, Graphics& gfx)
{
	Vei2& screenPos = gridPos * SpriteCodex::tileSize;
	switch (state)
	{
	case State::hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case State::reveald:
		if (isBomb) {
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
		else {
			SpriteCodex::DrawTileN(nNeighborsBombs, screenPos, gfx);
		}
		break;
	case State::flagged:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	default:
		break;
	}
}

void MineField::Tile::Reveal()
{
	assert(state == State::hidden);
	state = State::reveald;
}

void MineField::Tile::ToggleFlag()
{
	assert(state != State::reveald);

	if (state == State::flagged) {
		state = State::hidden;
	}
	else if (state == State::hidden) {
		state = State::flagged;
	}
}

void MineField::Tile::CalculateNeighbors(const Vei2& gridPos, const Tile tiles[])
{
	const int xStart = std::max(0, gridPos.x - 1);
	const int yStart = std::max(0, gridPos.y - 1);
	const int xEnd = std::min(width - 1, gridPos.x + 1);
	const int yEnd = std::min(height - 1, gridPos.y + 1);

	nNeighborsBombs = 0;
	for (Vei2 p = {xStart, yStart}; p.y <= yEnd; p.y++, p.x = xStart) {
		for (; p.x <= xEnd; p.x++) {
			if (tiles[p.y * width + p.x].HasBomb()) {
				nNeighborsBombs++;
			}
		}
	}
}

MineField::MineField(int nBombs)
{
	std::random_device rd;
	std::mt19937 rnd(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);

	for (; nBombs > 0; nBombs--) {
		Vei2 tilePos;
		Tile* tile;
		do {
			tilePos = { xDist(rnd), yDist(rnd) };
			tile = &tiles[tilePos.y * width + tilePos.x];
		} while (tile->HasBomb());
		tile->SpawnBomb();
	}

	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++, gridPos.x = 0) {
		for (; gridPos.x < width; gridPos.x++) {
			tiles[gridPos.y * width + gridPos.x].CalculateNeighbors(gridPos, tiles);
		}
	}

	// TEST CODE
	
	for (int nRevealed = 0; nRevealed < 100; nRevealed++) {
		Tile* tile;
		do {
			Vei2 tilePos = { xDist(rnd), yDist(rnd) };
			tile = &tiles[tilePos.y * width + tilePos.x];
		} while (tile->GetState() == Tile::State::reveald);
		tile->Reveal();
	}
	
}

void MineField::Draw(Graphics& gfx)
{
	gfx.DrawRect(background, Color(192, 192, 192));
	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++, gridPos.x = 0) {
		for (; gridPos.x < width; gridPos.x++) {
			tiles[gridPos.y * width + gridPos.x].Draw(gridPos, gfx);
		}
	}
}

void MineField::ProcessLMB(const Vei2& screenPos)
{
	const Vei2 gridPos = screenPos / SpriteCodex::tileSize;
	assert(gridPos.x >= 0 && gridPos.x <= width);
	assert(gridPos.y >= 0 && gridPos.y <= height);

	Tile& clickedTile = tiles[gridPos.y * width + gridPos.x];
	clickedTile.Reveal();
}

void MineField::ProcessRMB(const Vei2& screenPos)
{
	const Vei2 gridPos = screenPos / SpriteCodex::tileSize;
	assert(gridPos.x >= 0 && gridPos.x <= width);
	assert(gridPos.y >= 0 && gridPos.y <= height);

	Tile& clickedTile = tiles[gridPos.y * width + gridPos.x];
	clickedTile.ToggleFlag();
}