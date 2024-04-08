#include "tentacle_lib.h"
#include <windows.h>
#include <shellapi.h>
#include <imgui.h>
#include "WidgetLogin.h"
#include "url.h"
#include "GameData.h"
#include "Encode.h"

WidgetLogin::WidgetLogin()
{
	memset(username, 0, sizeof(username));
	memset(password, 0, sizeof(password));
}

void WidgetLogin::Update(float elapsedTime)
{
	if (xhr != nullptr)
	{
		if (xhr->IsDone())
		{
			HttpResponse response = xhr->GetResponse();
			if (response.status == 200) // ����
			{
				std::string token = response.data["token"];
				memset(GAME_DATA.GetSetting().token, 0, sizeof(GAME_DATA.GetSetting().token));
				token.copy(GAME_DATA.GetSetting().token, token.size());
				GAME_DATA.SaveGameSetting();

				state = STATE::SUCCESS;
			}
			else
			{
				// 200�ȊO: ���s
				state = STATE::FAIL;
			}
			delete xhr;
			xhr = nullptr;
		}
	}
}

void WidgetLogin::Render(const RenderContext& rc)
{
	switch (state)
	{
	case WidgetLogin::INPUT:
	case WidgetLogin::FAIL:
	{
		ImVec2 Center = ImGui::GetMainViewport()->Pos;
		Center.x += SCREEN_W * 0.5f - 150.0f;
		Center.y += SCREEN_H * 0.7f;
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowPos(
			Center,
			ImGuiCond_Always
		);

		ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_Once);

		if (ImGui::Begin("Window##Login", nullptr,
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoSavedSettings
		))
		{
			if (ImGui::BeginTable("Login", 2))
			{
				ImGui::TableSetupColumn("##text", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("##value", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text((char*)u8"���[�U�[");
				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1);
				ImGui::InputText("##username", username, sizeof(username));

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text((char*)u8"�p�X���[�h");
				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1);
				ImGui::InputText("##password", password, sizeof(password), ImGuiInputTextFlags_Password);

				if (state)
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(-1);
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
					ImGui::Text((char*)u8"���O�C�����s");
					ImGui::PopStyleColor();
				}

				ImGui::EndTable();
			}

			if (ImGui::Button((char*)u8"���O�C��"))
			{
				xhr = new HttpRequest(URL(Url::LOGIN));
				xhr->SetMethod(HttpRequest::METHOD::POST);
				xhr->SetData({
					{ "username", username },
					{ "password", password }
					});
				xhr->Send();
				state = STATE::LOADING;
			}
			ImGui::SameLine();
			if (ImGui::Button((char*)u8"�T�C���A�b�v"))
			{
				ShellExecute(0, 0, Encode::char_to_wchar(URL(Url::REGISTER_PAGE)), 0, 0, SW_SHOW);
			}
			ImGui::End();
		}
		break;
	}
	case WidgetLogin::LOADING:
	case WidgetLogin::SUCCESS:
		break;
	}
}