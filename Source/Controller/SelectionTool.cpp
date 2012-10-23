/*
 Copyright (C) 2010-2012 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "SelectionTool.h"

#include "Controller/ChangeEditStateCommand.h"
#include "Model/Brush.h"
#include "Model/EditStateManager.h"
#include "Model/Entity.h"
#include "Model/Face.h"
#include "Model/Picker.h"
#include "Utility/List.h"
#include "View/DocumentViewHolder.h"

namespace TrenchBroom {
    namespace Controller {
        bool SelectionTool::handleMouseUp(InputEvent& event) {
            if (!documentViewHolder().valid())
                return false;
            
            if (event.mouseButtons != MouseButtons::MBLeft)
                return false;
            
            Model::MapDocument& document = documentViewHolder().document();
            View::EditorView& view = documentViewHolder().view();
            
            Model::Hit* hit = event.pickResult->first(Model::HitType::ObjectHit, false, view.filter());
            Command* command = NULL;
            Model::EditStateManager& editStateManager = document.editStateManager();
            
            if (hit != NULL) {
                bool multi = event.modifierKeys() == ModifierKeys::MKCtrlCmd;
                
                if (hit->type() == Model::HitType::EntityHit) {
                    Model::Entity& entity = static_cast<Model::EntityHit*>(hit)->entity();
                    if (multi) {
                        if (entity.selected())
                            command = ChangeEditStateCommand::deselect(document, entity);
                        else
                            command = ChangeEditStateCommand::select(document, entity);
                    } else {
                        command = ChangeEditStateCommand::replace(document, entity);
                    }
                } else {
                    Model::Face& face = static_cast<Model::FaceHit*>(hit)->face();
                    Model::Brush& brush = *face.brush();
                    
                    if (brush.selected()) {
                        if (multi)
                            command = ChangeEditStateCommand::deselect(document, brush);
                        else
                            command = ChangeEditStateCommand::select(document, face);
                    } else if (face.selected()) {
                        if (multi)
                            command = ChangeEditStateCommand::deselect(document, face);
                        else
                            command = ChangeEditStateCommand::select(document, brush);
                    } else {
                        if (multi) {
                            if (editStateManager.selectionMode() == Model::EditStateManager::SMFaces)
                                command = ChangeEditStateCommand::select(document, face);
                            else
                                command = ChangeEditStateCommand::select(document, brush);
                        } else {
                            if (editStateManager.selectionMode() == Model::EditStateManager::SMFaces)
                                command = ChangeEditStateCommand::replace(document, face);
                            else
                                command = ChangeEditStateCommand::replace(document, brush);
                        }
                    }
                }
            } else {
                command = ChangeEditStateCommand::deselectAll(document);
            }

            if (command != NULL) {
                postCommand(command);
                return true;
            }
            
            return false;
        }
        
        bool SelectionTool::handleScrolled(InputEvent& event) {
            if (!documentViewHolder().valid())
                return false;
            
            if (event.modifierKeys() != ModifierKeys::MKCtrlCmd &&
                event.modifierKeys() != (ModifierKeys::MKCtrlCmd | ModifierKeys::MKShift))
                return false;
            
            Model::MapDocument& document = documentViewHolder().document();
            View::EditorView& view = documentViewHolder().view();
            
            Model::EditStateManager& editStateManager = document.editStateManager();
            if (editStateManager.selectionMode() == Model::EditStateManager::SMFaces)
                return false;
            
            const Model::HitList hits = event.pickResult->hits(Model::HitType::ObjectHit, view.filter());
            if (hits.empty())
                return false;
            
            Model::EntityList entities = editStateManager.selectedEntities();
            Model::BrushList brushes = editStateManager.selectedBrushes();
            
            Command* command = NULL;
            bool appendSelection = event.modifierKeys() == (ModifierKeys::MKCtrlCmd | ModifierKeys::MKShift);
            bool foundSelection = false;

            for (size_t i = 0; i < hits.size(); i++) {
                Model::Hit* hit = hits[i];
                if (hit->type() == Model::HitType::EntityHit) {
                    Model::Entity& entity = static_cast<Model::EntityHit*>(hit)->entity();
                    if (entity.selected()) {
                        if (!appendSelection)
                            Utility::erase(entities, &entity);
                        foundSelection = true;
                    }
                } else {
                    Model::Face& face = static_cast<Model::FaceHit*>(hit)->face();
                    Model::Brush& brush = *face.brush();
                    if (brush.selected()) {
                        if (!appendSelection)
                            Utility::erase(brushes, &brush);
                        foundSelection = true;
                    }
                }
                
                if (foundSelection) {
                    Model::Hit* hit = NULL;
                    if (event.scroll() > 0.0f && i < hits.size() - 1)
                        hit = hits[i + 1];
                    else if (event.scroll() < 0.0f && i > 0)
                        hit = hits[i - 1];
                    
                    if (hit != NULL) {
                        if (hit->type() == Model::HitType::EntityHit) {
                            Model::Entity& entity = static_cast<Model::EntityHit*>(hit)->entity();
                            if (!entity.selected()) {
                                entities.push_back(&entity);
                                command = ChangeEditStateCommand::replace(document, entities, brushes);
                            }
                        } else {
                            Model::Face& face = static_cast<Model::FaceHit*>(hit)->face();
                            Model::Brush& brush = *face.brush();
                            if (!brush.selected()) {
                                brushes.push_back(&brush);
                                command = ChangeEditStateCommand::replace(document, entities, brushes);
                            }
                        }
                    }
                }
            }
            
            if (!foundSelection) {
                Model::Hit* hit = hits[0];
                if (hit->type() == Model::HitType::EntityHit) {
                    Model::Entity& entity = static_cast<Model::EntityHit*>(hit)->entity();
                    entities.push_back(&entity);
                } else {
                    Model::Face& face = static_cast<Model::FaceHit*>(hit)->face();
                    Model::Brush& brush = *face.brush();
                    brushes.push_back(&brush);
                }
                command = ChangeEditStateCommand::replace(document, entities, brushes);
            }
            
            if (command != NULL) {
                postCommand(command);
                return true;
            }
            
            return false;
        }
    }
}