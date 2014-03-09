#include <windows.h>
#include <deque>

int main()
{
	for (int i = 1; i < 10; ++i)
	{
		if (!::RegisterHotKey(0, i + 1, MOD_WIN, VK_F1 + i)
			|| !::RegisterHotKey(0, i + 11, MOD_WIN | MOD_CONTROL, VK_F1 + i))
		{
			return 1;
		}
	}

	if (!::RegisterHotKey(0, 101, MOD_WIN, 'N')
		|| !::RegisterHotKey(0, 103, MOD_WIN | MOD_SHIFT, 'N')
		|| !::RegisterHotKey(0, 102, MOD_WIN | MOD_SHIFT | MOD_CONTROL, 'N'))
	{
		return 1;
	}

	std::deque<HWND> windows;
	HWND num_windows[10] = {};

	MSG msg;
	while (::GetMessage(&msg, 0, WM_HOTKEY, WM_HOTKEY) > 0)
	{
		switch (msg.wParam)
		{
		case 101:
			{
				GUITHREADINFO gti = { sizeof gti };
				if (::GetGUIThreadInfo(0, &gti) && gti.hwndActive != 0)
				{
					windows.push_back(gti.hwndActive);
					if (windows.size() > 100)
						windows.pop_front();
					::PostMessage(gti.hwndActive, WM_SYSCOMMAND, SC_MINIMIZE, -1);
				}
			}
			break;
		case 102:
			return 0;
		case 103:
			if (!windows.empty())
			{
				::PostMessage(windows.back(), WM_SYSCOMMAND, SC_RESTORE, -1);
				windows.pop_back();
			}
			break;
		default:
			if (msg.wParam > 0 && msg.wParam <= 10)
			{
				if (num_windows[msg.wParam - 1] != 0)
				{
					if (!IsWindow(num_windows[msg.wParam - 1]))
						num_windows[msg.wParam - 1] = 0;
					else if (::IsIconic(num_windows[msg.wParam - 1]))
						::PostMessage(num_windows[msg.wParam - 1], WM_SYSCOMMAND, SC_RESTORE, -1);
					else if (::GetForegroundWindow() != num_windows[msg.wParam - 1])
						::SetForegroundWindow(num_windows[msg.wParam - 1]);
					else
						::PostMessage(num_windows[msg.wParam - 1], WM_SYSCOMMAND, SC_MINIMIZE, -1);
				}
				break;
			}
			if (msg.wParam > 10 && msg.wParam <= 20)
			{
				GUITHREADINFO gti = { sizeof gti };
				if (::GetGUIThreadInfo(0, &gti) && gti.hwndActive != 0)
					num_windows[msg.wParam - 11] = gti.hwndActive;
				break;
			}
			return 3;
		}
	}

	return 2;
}
