/******************************************************************************************************************************************/
// File : WorkBarComponent.cpp
// Purpose : Implementing the GUI view tools
/******************************************************************************************************************************************/

#include "WorkBarComponent.hpp"

namespace Views
{
	WorkBarComponent::~WorkBarComponent()
	{
		if (m_state_service)
		{
			m_state_service.reset();
		}

		if (m_framebuffer_service)
		{
			m_framebuffer_service.reset();
		}

		if (!tabs_size.empty())
		{
			tabs_size.clear();
		}

		if (m_parent_view_model)
		{
			m_parent_view_model.reset();
		}
	}
	WorkBarComponent::WorkBarComponent(std::shared_ptr<ViewModels::IViewModel> parent) : show_color_picker(false), item_current(-1), current_tab(0), m_selected_skybox(""), is_there_light_directional_source(false)
	{
		m_parent_view_model = parent;
		IoC::Container::Container* container = IoC::Container::Container::GetInstanceContainer();
		if (container)
		{
			m_state_service = container->GetReference<Services::StateService>();
			if (!m_state_service)
			{
				SQ_CLIENT_ERROR("Class {} in function {} : State service is not referenced yet", __FILE__, __FUNCTION__);
			}
			else
			{
				w_width = 400;
			}

			m_framebuffer_service = container->GetReference<Services::FramebufferService>();
			if (!m_framebuffer_service)
			{
				SQ_CLIENT_ERROR("Class {} in function {} : Framebuffer service is not referenced yet", __FILE__, __FUNCTION__);
			}

			m_loader_service = container->GetReference<Services::LoaderService>();
			if (!m_loader_service)
			{
				SQ_CLIENT_ERROR("Class {} in function {} : Loader service is not referenced yet", __FILE__, __FUNCTION__);
			}

			m_runtime_service = container->GetReference<Services::RunTimeService>();
			if (!m_runtime_service)
			{
				SQ_CLIENT_ERROR("Class {} in function {} : Runtime service is not referenced yet", __FILE__, __FUNCTION__);
			}

			m_physics_service = container->GetReference<Services::PhysicsService>();
			if (!m_physics_service)
			{
				SQ_CLIENT_ERROR("Class {} in function {} : Physics service is not referenced yet", __FILE__, __FUNCTION__);

			}

			m_shader_service = container->GetReference<Services::ShaderService>();
			if (!m_shader_service)
			{
				SQ_CLIENT_ERROR("Class {} in function {} : Shader service is not referenced yet", __FILE__, __FUNCTION__);

			}
		}

		tabs_size.push_back(ImVec2(0, 350));
		tabs_size.push_back(ImVec2(0, 700));
		tabs_size.push_back(ImVec2(0, 300));
		tabs_size.push_back(ImVec2(0, 100));
	}
	void WorkBarComponent::Render()
	{
		if (m_state_service && m_state_service->getContinued())
		{
			w_height = m_state_service->getHeight();
			ImGui::SetNextWindowPos(ImVec2((float)((m_state_service->getWidth()) - w_width), 0));
			ImGui::SetNextWindowSize(ImVec2((float)w_width, (float)w_height));
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoScrollbar
				| ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoFocusOnAppearing
				| ImGuiWindowFlags_NoBringToFrontOnFocus;
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			ImGuiWindowFlags window_flags2 = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar;

			if (ImGui::Begin("WorkBar", nullptr, window_flags))
			{
				this->RenderGeneralFunctionnalities(window_flags2);
				this->RenderCustomizeSelectedCpSection(tab_bar_flags, window_flags2);
				this->RenderDebugFunctionnalities(window_flags2);

				ImGui::End();
			}	
		}
	}
	void WorkBarComponent::RenderGeneralFunctionnalities(ImGuiWindowFlags window_flags2)
	{
		bool show_confirm = m_state_service->getShowConfirm();

		if (ImGui::BeginChild("ChildGeneralFun", ImVec2(0, 250), true, window_flags2))
		{
			const char* items[] = { "Triangle", "Square", "Cube", "Sphere", "Cube textured", "Square textured", "Triangle textured", "Sphere textured"};
			ImGui::BulletText("Add new :");
			if (ImGui::Combo(" ", &item_current, items, IM_ARRAYSIZE(items)))
			{

				if (m_parent_view_model)
				{	
					m_parent_view_model->AddCommand(std::make_unique<Commands::ModifySceneCommand>(Enums::SceneModifier::ADDCOMPONENT, static_cast<Enums::RendererType>(item_current)));
					m_state_service->setConfirmMessage("You are about to add a new component. Are you sure ?");
					show_confirm = true;
				}
				item_current = -1;
			}

			ImGui::Separator();

			if (m_state_service->GetScene())
			{
				ImGui::BulletText("Change skybox :");
				int img_size = 50;
				std::map<std::string, unsigned int> available_skybox = m_state_service->getAvailableSkybox();
				m_selected_skybox = m_state_service->GetScene()->GetSelectedSkybox();
				for (std::map<std::string, unsigned int>::iterator it = available_skybox.begin(); it != available_skybox.end(); it++)
				{
					if (it->first == m_selected_skybox)
					{
						ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
						ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
						ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
						ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 1.f); // 50% opaque white
						ImGui::Image((ImTextureID)(intptr_t)it->second, ImVec2((float)img_size, (float)img_size), uv_max, uv_min, tint_col, border_col);
					}
					else
					{
						ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
						ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
						ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);     // Black background
						ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
						if (ImGui::ImageButton((ImTextureID)(intptr_t)it->second, ImVec2((float)img_size, (float)img_size), uv_max, uv_min, 2, bg_col, tint_col))
						{
							m_parent_view_model->AddCommand(std::make_unique<Commands::ModifySceneCommand>(Enums::SceneModifier::CHANGESKYBOX, it->first));
							m_state_service->setConfirmMessage("You are about to change the skybox. Are you sure ?");
							show_confirm = true;
						}
					}
					ImGui::SameLine((float)img_size + 20.f);

				}
				ImGui::Text(" ");
				for (std::map<std::string, unsigned int>::iterator it = available_skybox.begin(); it != available_skybox.end(); it++)
				{
					ImGui::Text(it->first.c_str());
					ImGui::SameLine((float)img_size + 30.f);
				}
				ImGui::Text(" ");

				ImGui::Separator();

				ImGui::BulletText("Directional light");

				if (m_state_service && m_state_service->GetScene() && m_physics_service && m_shader_service)
				{
					is_there_light_directional_source = m_state_service->GetScene()->GetIsThereDirectionLight();
					if (!is_there_light_directional_source)
					{
						if (ImGui::Button("Add directional light source"))
						{
							m_state_service->GetScene()->SetIsThereDirectionLight(true);
							m_state_service->GetScene()->SetDirectionLight(glm::vec3(0.f, -1.f, 0.f));
							m_physics_service->RemoveLightSources();
							m_physics_service->SetLightSourcesGeneralParameters();
							m_shader_service->PassLightsParametersToSSBO(m_physics_service->GetLigthSources());
						}
					}
					else
					{
						ImGui::PushID(0);
						ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
						if (ImGui::Button("Delete directional light source"))
						{
							m_state_service->GetScene()->SetIsThereDirectionLight(false);
						}
						ImGui::PopStyleColor(3);
						ImGui::PopID();

						float theta = m_physics_service->GetTheta();
						float phi = m_physics_service->GetPhi();

						if (ImGui::SliderFloat("Polar angle", &theta, 0, glm::pi<float>(), "%.3f"))
						{
							m_state_service->GetScene()->SetDirectionLight(m_physics_service->UpdateDirectionalLight(Enums::AngleToUpdate::POLAR, theta, phi));
						}

						if (ImGui::SliderFloat("Azymuthal angle", &phi, 0, glm::two_pi<float>(), "%.3f"))
						{
							m_state_service->GetScene()->SetDirectionLight(m_physics_service->UpdateDirectionalLight(Enums::AngleToUpdate::AZYMUTH, theta, phi));
						}

					}
				}
			}
			ImGui::EndChild();
		}

		m_state_service->setShowConfirm(show_confirm);
	}

	void WorkBarComponent::RenderDebugFunctionnalities(ImGuiWindowFlags window_flags2)
	{
		if (m_framebuffer_service && m_state_service && m_state_service->getConfigs() && m_state_service->getConfigs()->GetRenderDebug() && m_runtime_service)
		{
			if (ImGui::BeginChild("ChildDebugFun", ImVec2(0, 500), true, window_flags2))
			{
				std::string text_bloom_debug = m_state_service->getConfigs()->GetBloom() ? "Bloom back buffer" : "Bloom back buffer (none)";
				ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
				ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
				ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
				ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
				ImGui::Text(text_bloom_debug.c_str());
				if (m_state_service->getConfigs()->GetBloom())
				{
					ImGui::Image((ImTextureID)(intptr_t)m_framebuffer_service->GetTextureId(1), ImVec2((float)w_width - 40, 210), uv_max, uv_min, tint_col, border_col);
				}
				std::string text_depth_debug = m_state_service->getConfigs()->GetDepth() ? "Depth back buffer" : "Depth back buffer (none)";
				ImGui::Text(text_depth_debug.c_str());
				if (m_state_service->getConfigs()->GetDepth())
				{
					ImGui::Image((ImTextureID)(intptr_t)m_framebuffer_service->GetDephtTextureId(), ImVec2((float)w_width - 40, 210), uv_max, uv_min, tint_col, border_col);
				}
				ImGui::EndChild();
			}
		}
		
	}

	int WorkBarComponent::GetPowerIndex(int specular_shininess)
	{
		int exponent = 0;

		while (specular_shininess > 1)
		{
			specular_shininess /= 2;
			++exponent;
		}

		return exponent;
	}

	void WorkBarComponent::RenderCustomizeSelectedCpSection(ImGuiTabBarFlags tab_bar_flags, ImGuiWindowFlags window_flags2)
	{
		if (m_state_service && m_state_service->getContinued())
		{

			ImGuiStyle& style = ImGui::GetStyle();
			frame_rounding_save = style.FrameRounding;
			ImVec2 window_padding_save = style.WindowPadding;

			style.FrameRounding = 20.f;
			style.WindowPadding.y = 30.f;
			std::shared_ptr<Component::IComponent> selected_renderer = m_state_service->getSelectedComponent();
			if (selected_renderer)
			{
				style.WindowPadding = window_padding_save;
				if (ImGui::BeginChild("ChildSelectedCpt", (current_tab < tabs_size.size() ? tabs_size.at(current_tab) : ImVec2(0, 300)), true, window_flags2))
				{
					if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
					{
						if (ImGui::BeginTabItem("Properties"))
						{
							current_tab = 0;
							m_state_service->setPopupHovered(ImGui::IsWindowHovered());
							this->RenderPropertiesTab(selected_renderer);
							ImGui::EndTabItem();
						}
						if (ImGui::BeginTabItem("Appearance"))
						{
							current_tab = 1;
							m_state_service->setPopupHovered(ImGui::IsWindowHovered());
							this->RenderAppearanceTab(selected_renderer);
							ImGui::EndTabItem();
						}
						if (ImGui::BeginTabItem("Light"))
						{
							current_tab = 2;
							m_state_service->setPopupHovered(ImGui::IsWindowHovered());
							this->RenderLightTab(selected_renderer);
							ImGui::EndTabItem();
						}
						if (ImGui::BeginTabItem("Others"))
						{
							current_tab = 3;
							m_state_service->setPopupHovered(ImGui::IsWindowHovered());
							this->RenderOtherFunTab(selected_renderer);
							ImGui::EndTabItem();
						}
						ImGui::EndTabBar();
					}

					ImGui::EndChild();
				}

				selected_renderer.reset();
			}

			style.FrameRounding = frame_rounding_save;
			style.WindowPadding = window_padding_save;

		}
	}

	void WorkBarComponent::RenderPropertiesTab(std::shared_ptr<Component::IComponent> selected_renderer)
	{
		if (m_state_service && selected_renderer)
		{	
			glm::vec3 size = selected_renderer->GetSize();
			glm::vec3 position = selected_renderer->GetPosition();
			float global_size = selected_renderer->GetSize().x;
			float angle_1 = selected_renderer->GetAngle1();
			float angle_2 = selected_renderer->GetAngle2();
			float angle_3 = selected_renderer->GetAngle3();

			ImGui::BulletText("Position : ");
			ImGui::SliderFloat("X - Axis", &position.x, -5.0f, 5.0f, "%.3f");
			ImGui::SliderFloat("Y - Axis", &position.y, -5.0f, 5.0f, "%.3f");
			ImGui::SliderFloat("Z - Axis", &position.z, -5.0f, 5.0f, "%.3f");

			selected_renderer->SetPosition(position);

			ImGui::BulletText("Size : ");
			if (ImGui::SliderFloat("Width", &size.x, 0.0f, 5.0f, "%.3f") || ImGui::SliderFloat("Height", &size.y, 0.0f, 5.0f, "%.3f") || ImGui::SliderFloat("Depth", &size.z, 0.0f, 5.0f, "%.3f"))
			{
				selected_renderer->SetSize(size);
			}
			if (ImGui::SliderFloat("Global", &global_size, 0.0f, 5.0f, "%.3f"))
			{
				selected_renderer->SetSize(global_size);
			}

			ImGui::BulletText("Orientation : ");
			if (ImGui::SliderFloat("X axis", &angle_1, 0.f, 180.f, "%.3f"))
			{
				selected_renderer->SetAngle1(angle_1);
			}

			if (ImGui::SliderFloat("Y axis", &angle_2, 0.f, 180.f, "%.3f"))
			{
				selected_renderer->SetAngle2(angle_2);
			}

			if (ImGui::SliderFloat("Z axis", &angle_3, 0.f, 180.f, "%.3f"))
			{
				selected_renderer->SetAngle3(angle_3);
			}
		}
	}

	void WorkBarComponent::RenderAppearanceTab(std::shared_ptr<Component::IComponent> selected_renderer)
	{
		if (m_state_service && selected_renderer && m_loader_service)
		{
			glm::vec4 color = selected_renderer->GetBackgroundColor();
			glm::vec4 ref_color = m_state_service->getRefColor();

			ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaPreviewHalf
				| ImGuiColorEditFlags_AlphaBar
				| ImGuiColorEditFlags_PickerHueWheel
				| ImGuiColorEditFlags_NoInputs;
			ImGui::BulletText("Background color : ");
			ImGui::ColorPicker4("New Color##4", (float*)&color, flags, (float*)&ref_color);

			selected_renderer->SetBackgroundColor(color);

			

			switch (selected_renderer->GetType())
			{
			case Enums::RendererType::CUBE_TEXTURED:
			case Enums::RendererType::SQUARE_TEXTURED:
			case Enums::RendererType::TRIANGLE_TEXTURED:
			case Enums::RendererType::SPHERE_TEXTURED:
			{
				ImGui::Text(" ");
				ImGui::Separator();

				float image_size = 50;
				int place_taken = 0;
				std::map<std::string, unsigned int> available_textures = m_state_service->GetAvailableTextures();
				for (std::map<std::string, unsigned int >::iterator it = available_textures.begin(); it != available_textures.end(); it++)
				{
					if (it->second == selected_renderer->GetTextureId())
					{
						ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
						ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
						ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
						ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 1.f); // 50% opaque white
						ImGui::Image((ImTextureID)(intptr_t)it->second, ImVec2(image_size, image_size), uv_max, uv_min, tint_col, border_col);
						place_taken += (int)image_size;
					}
					else
					{
						ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
						ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
						ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);     // Black background
						ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
						if (ImGui::ImageButton((ImTextureID)(intptr_t)it->second, ImVec2(image_size, image_size), uv_max, uv_min, 2, bg_col, tint_col))
						{

							m_loader_service->LoadTexture(std::dynamic_pointer_cast<Component::TexturedComponent> (selected_renderer), it->first);
							selected_renderer->SetMixeTextureColor(false);
							selected_renderer->SetTextureName(it->first);
						}
						place_taken += (int)image_size;
					}

					if (place_taken < 300)
					{
						ImGui::SameLine();
					}
					else
					{
						ImGui::Text(" ");
						place_taken = 0;
					}
					

				}

				ImGui::Text(" ");
				ImGui::Separator();
				bool mixe_texture = selected_renderer->GetMixeTextureColor();
				if (ImGui::Checkbox("Mixe texture and color", &mixe_texture))
				{
					selected_renderer->SetMixeTextureColor(mixe_texture);
				}
			}
				break;
			default:
				break;
			}
		}
	}

	void WorkBarComponent::RenderLightTab(std::shared_ptr<Component::IComponent> selected_renderer)
	{
		if (selected_renderer)
		{
			if (!is_there_light_directional_source && m_physics_service && m_shader_service)
			{
				bool is_light_source = selected_renderer->GetIsALightSource();
				if (ImGui::Checkbox("Is a light source", &is_light_source))
				{
					selected_renderer->SetIsALigthSource(is_light_source);
					m_physics_service->SetLightSourcesGeneralParameters();
					m_physics_service->SetLightsAttenuationsParameters();
					m_shader_service->PassLightsParametersToSSBO(m_physics_service->GetLigthSources());
				}
			}

			if (m_physics_service->GetLigthSources().size() > 0 || is_there_light_directional_source)
			{
				ImGui::Separator();

				if (!selected_renderer->GetIsALightSource())
				{
					float ambiant = selected_renderer->GetAmbiantOcclusion();
					if (ImGui::SliderFloat("Ambiant occlusion", &ambiant, 0.f, 0.9f, "%.3f"))
					{
						selected_renderer->SetAmbiantOcclusion(ambiant);
					}

					int specular_shininess = selected_renderer->GetSpecularShininess();
					int index = this->GetPowerIndex(specular_shininess);
					if (ImGui::SliderInt("Specular Shininess", &index, 1, 8, std::to_string(specular_shininess).c_str()))
					{
						specular_shininess = (int)std::pow(2, index);
						selected_renderer->SetSpecularShininess(specular_shininess);
					}

					float specular_strength = selected_renderer->GetSpecularStrength();
					if (ImGui::SliderFloat("Specular Strength", &specular_strength, 0.1f, 1.f, "%.3f"))
					{
						selected_renderer->SetSpecularStrength(specular_strength);
					}
				}
				else
				{
					const char* light_types[Enums::LightType::NBLIGHTTYPE] = {"Normal light", "Point light", "Spot light" };
					int light_type_index = selected_renderer->GetLightType();
					const char* light_type = (light_type_index >= 0 && light_type_index < Enums::LightType::NBLIGHTTYPE) ? light_types[light_type_index] : "Unknown";
					if (ImGui::SliderInt("Light type", &light_type_index, 0, Enums::LightType::NBLIGHTTYPE - 1, light_type))
					{
						selected_renderer->SetLightType(static_cast<Enums::LightType>(light_type_index));
					}

					float intensity = selected_renderer->GetIntensity();

					if (ImGui::SliderFloat("Intensity", &intensity, 0.f, 1.f , "%.3f"))
					{
						selected_renderer->SetIntensity(intensity);
					}

					if (selected_renderer->GetLightType() == Enums::LightType::SPOTLIGHT)
					{
						ImGui::Separator();

						bool is_attenuation = selected_renderer->GetIsAttenuation();
						if (ImGui::Checkbox("Use attenuation", &is_attenuation))
						{
							selected_renderer->SetIsAttenuation(is_attenuation);
						}

						float theta = m_physics_service->GetTheta();
						float phi = m_physics_service->GetPhi();

						if (ImGui::SliderFloat("Polar angle", &theta, 0, glm::pi<float>(), "%.3f"))
						{
							selected_renderer->SetDirection(m_physics_service->UpdateDirectionalLight(Enums::AngleToUpdate::POLAR, theta, phi));
						}

						if (ImGui::SliderFloat("Azymuthal angle", &phi, 0, glm::two_pi<float>(), "%.3f"))
						{
							selected_renderer->SetDirection(m_physics_service->UpdateDirectionalLight(Enums::AngleToUpdate::AZYMUTH, theta, phi));
						}

						ImGui::Separator();

						float cut_off = selected_renderer->GetCutOff();
						if (ImGui::SliderFloat("Light Cut off", &cut_off, 0, 20.f, "%.3f"))
						{
							selected_renderer->SetCutOff(cut_off);
						}

						ImGui::Separator();

						float outer_cut_off = selected_renderer->GetOuterCutOff();
						if (ImGui::SliderFloat("Outer Cut off", &outer_cut_off, 0, 5.f, "%.3f"))
						{
							selected_renderer->SetOuterCutOff(outer_cut_off);
						}
					}
				}
			}
		}
	}

	void WorkBarComponent::RenderOtherFunTab(std::shared_ptr<Component::IComponent> selected_renderer)
	{
		if (m_state_service && selected_renderer)
		{
			bool show_confirm = m_state_service->getShowConfirm();
			ImGui::BulletText("Delete component :");
			ImGui::SameLine();
			ImGui::PushID(0);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
			if (ImGui::Button("Delete", ImVec2(100, 20)))
			{
				if (m_parent_view_model)
				{
					m_parent_view_model->AddCommand(std::make_unique<Commands::ModifySceneCommand>(Enums::SceneModifier::DELETECOMPONENT));
					m_state_service->setConfirmMessage("You are about to delete a component. Are you sure ?");
					show_confirm = true;
				}
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();

			ImGui::BulletText("Copy component :");
			ImGui::SameLine();
			if (ImGui::Button("Copy", ImVec2(100, 20)))
			{
				if (m_parent_view_model)
				{
					m_parent_view_model->AddCommand(std::make_unique<Commands::ModifySceneCommand>(Enums::SceneModifier::COPYCOMPONENT, selected_renderer));
					m_state_service->setConfirmMessage("You are about to copy a component. Are you sure ?");
					show_confirm = true;
				}
			}

			m_state_service->setShowConfirm(show_confirm);
		}
	}
	
}