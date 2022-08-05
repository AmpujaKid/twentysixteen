#include "HookManager.h"

CHooker* ClientModeVmt;
CHooker* ClientVmt;
CHooker* PanelVmt;
CHooker* PredictionVmt;
CHooker* MaterialSystemVmt;
CHooker* GameEventManagerVmt;
CHooker* ModelRenderVmt;
CHooker* SurfaceVmt;
CHooker* EngineSoundVmt;

#include "CreateMove.h"
#include "OverrideView.h"
#include "RunCommand.h"
#include "FrameStageNotify.h"
#include "FindMaterial.h"
#include "FireEventClientSide.h"
#include "DrawModelExecute.h"
#include "PaintTraverse.h"
#include "PlaySong.h"