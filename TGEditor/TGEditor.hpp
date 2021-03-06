#pragma once

#include <Stdbase.hpp>

#include <model/FBX_Loader.hpp>
#include <TGEngine.hpp>
#include <drawlib/DrawLib.hpp>
#include <io/LoadFont.hpp>
#include <pipeline/CommandBuffer.hpp>
#include <util/TGVertex.hpp>
#include "game_content/Mesh.hpp"
#include "game_content/FirstPersonCamera.hpp"
#include "game_content/StaticCamera.hpp"
#include "ui/UITextureComponent.hpp"
#include "ui/UI.hpp"

extern Texture tex1;
extern Texture tex2;
extern Mesh mesh;
extern Mesh mesh2;
extern Font arial;
extern Camera camera;
extern tg_ui::UIEntity entity;

struct Editor : App
{
	Editor() : App() {};
};

void init();

void drawloop(IndexBuffer* ibuffer, VertexBuffer* verticies);

int main();