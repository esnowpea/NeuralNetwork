#include "image.h"

ImageData*		agLoadImage(ILconst_string FileName)			//FileName - указатель на строку с именем файла
{
	ILuint*		Texture = new ILuint;				//Указатель на текстуру
	ImageData*	Result = NULL;						//Изображение

	ilInit();										//И и и л з ц я D v L
	ilEnable(IL_ORIGIN_SET);						// н ц а и а и е I
	ilGenImages(1, Texture);						//Генерируем текстуру
	ilBindImage(*Texture);							//Делаем текстуру текущей

	if(ilLoadImage(FileName) == true)				//Пытаемся загрузить изображение
	{												// если да, то
		Result = new ImageData();					//Создаём изображение

		Result -> Width = ilGetInteger(IL_IMAGE_WIDTH);			//Получаем ширину изображения
		Result -> Height = ilGetInteger(IL_IMAGE_HEIGHT);		//Получаем высоту изображения
		Result -> Bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);	//Получаем количество байт на пиксель
		Result -> Stride = Result -> Width * Result -> Bpp;		//Вычислем количество байт в одной строке пикселей

		Result -> Data = new unsigned char[Result -> Width * Result -> Height * Result -> Bpp]; //Создаём указатель на данные изображения


		memcpy(Result -> Data, ilGetData(), ilGetInteger(IL_IMAGE_SIZE_OF_DATA)); //Копируем данные из памяти в наше изображение

		Result -> BaseInfo.Type = ilGetInteger(IL_IMAGE_TYPE);		//Получаем тип данных в памяти
		Result -> BaseInfo.Format = ilGetInteger(IL_IMAGE_FORMAT);	//Получаем формат пикселей
		Result -> BaseInfo.Depth = ilGetInteger(IL_IMAGE_DEPTH);	//Получаем глубину изображения

		if(Result -> BaseInfo.Format == IL_RGB)	//Если формат пикселей BGR, то
		{										//данные располагаются в памяти
			Result -> R_idx = 0;				//так ->
			Result -> G_idx = 1;				//0|1|2|0|1|2|0|1|2|0|1
			Result -> B_idx = 2;				//R|G|B|R|G|B|R|G|B|R|G
		}
		else if(Result -> BaseInfo.Format == IL_RGBA)	//Если формат пикселей RGBA,то
		{												//данные располагаются в памяти
			Result -> R_idx = 0;						//так ->
			Result -> G_idx = 1;						//0|1|2|3|0|1|2|3|0|1|2|3|0
			Result -> B_idx = 2;						//R|G|B|A|R|G|B|A|R|G|B|A|R
			Result -> A_idx = 3;
		}
		else if(Result -> BaseInfo.Format == IL_BGR)	//Если формат пикселей BGR,то
		{												//данные располагаются в памяти
			Result -> R_idx = 2;						//так ->
			Result -> G_idx = 1;						//0|1|2|0|1|2|0|1|2|0|1|2
			Result -> B_idx = 0;						//B|G|R|B|G|R|B|G|R|B|G|R
		}
		else if(Result -> BaseInfo.Format == IL_BGRA)	//Если формат пикселей BGRA, то
		{												//данные располагаются в памяти
			Result -> R_idx = 2;						//так ->
			Result -> G_idx = 1;						//0|1|2|3|0|1|2|3|0|1|2|3|0
			Result -> B_idx = 0;						//B|G|R|A|B|G|R|A|B|G|R|A|B
			Result -> A_idx = 3;
		}
		else
		{
			MessageBoxA(0, "Unsuitable pixel format", "Error", MB_OK | MB_ICONERROR);
		}
	}
	else
	{												//Если нет, то выдаём сообщение об ошибке
		MessageBoxA(0, "Could not load image", "Error", MB_OK | MB_ICONEXCLAMATION);
	}


	ilDeleteImages(1, Texture);	//Удаляем текстуру ->
	delete Texture;				//->|

	return Result;
}


bool agSaveImage(ImageData* IMG, ILconst_string FileName, ImageType IType)	//IMG - изображение, FileName - имя сохраняемого файла, IType - расширение файла
{
	bool	Res = 0;							//Результат функции
	if(IMG != NULL && strlen((char*)FileName) > 0)	//Если изображение не пустое и длинна имени файла больше нуля,то
	{
		ILuint * Texture = new ILuint;		//Указатель на текстуру
		ilInit();							//Инициализация DevIL
		ilGenImages(1, Texture);			//Генерируем текстуру
		ilBindImage(*Texture);				//Делаем её текущей

		ilTexImage(IMG -> Width,IMG -> Height, IMG -> BaseInfo.Depth,IMG -> Bpp,IMG -> BaseInfo.Format,IMG -> BaseInfo.Type, IMG -> Data);	//Закидываем данные в память

		Res = ilSave(IType, FileName);		//И сохраняем изображение
		ilDeleteImages(1, Texture);			//Удаляем текстуру ->
		delete Texture;						//->|
	}
	else
	{										//Иначе,выдаем сообщене об ошибке
		MessageBoxA(0, "Pointer to the image is empty or the wrong name of the saved file", "Error", MB_OK | MB_ICONERROR);
	}
	return Res;
}

