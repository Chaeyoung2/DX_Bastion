#pragma once

class CGameObject;
typedef std::list<CGameObject*> OBJLIST;

class CGameObject;
typedef std::list<CGameObject*>::iterator OBJITER;


#define GET_TIME CTimeMgr::GetInstance()->GetTime()

#define MSG_BOX(text) MessageBox(NULL, text, NULL, MB_OK)
#define TAGMSG_BOX(_tag,_message)	MessageBox(NULL, _message, _tag, MB_OK)
