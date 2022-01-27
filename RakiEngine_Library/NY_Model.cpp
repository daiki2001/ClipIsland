#include "NY_Model.h"
#include "TexManager.h"

void NY_Model3D::LoadObjModel(const char *modelName)
{
	//FileStream
	ifstream file;
	/*   ���f�����݂̂Ŏw��\�ɂ��邽�߂̏����i�}�e���A���ǂݍ��݂��y�ɂ��邽�߁j   */
	string fileName		= modelName;
	string folderName	= fileName + "/";
	string modelpath	= "Resources/objFiles/" + folderName + fileName;
	string matarialpath = "Resources/objFiles/" + folderName;

	//obj�t�@�C���I�[�v��
	file.open(modelpath + ".obj");
	//�I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//���l�i�[
	vector<XMFLOAT3> positions;
	vector<XMFLOAT3> normals;
	vector<XMFLOAT2> texcoords;

	//1�s���ǂݍ���
	string line;
	while (getline(file, line)) {

		//1�s���̕�������X�g���[���ɕϊ����ēǂ݂₷������
		istringstream line_stream(line);

		//���p�X�y�[�X�ōs�̐擪������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪�� v => ���_���W
		if (key == "v") {
			//xyz���W�ǂݍ���
			XMFLOAT3 loadedPos{};
			line_stream >> loadedPos.x;
			line_stream >> loadedPos.y;
			line_stream >> loadedPos.z;
			//���W�f�[�^�ǉ�
			positions.emplace_back(loadedPos);
		}

		//�擪�� f => �|���S���C���f�b�N�X
		if (key == "f") {
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ēǂ݂₷������
				istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);//�X���b�V����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);//�X���b�V����΂�
				index_stream >> indexNormal;
				//���_�f�[�^�ǉ�
				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);
				//���_�C���f�b�N�X�ɒǉ�
				indices.emplace_back((unsigned short)indices.size());
			}
		}

		//�擪�� vt => �e�N�X�`��
		if (key == "vt") {
			//U,V�����ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V�������]
			texcoord.y = 1.0f - texcoord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}

		//�擪�� vn => �@���x�N�g��
		if (key == "vn") {
			//x,y,z�����ǂݍ���
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�@���x�N�g���Ƀf�[�^�ǉ�
			normals.emplace_back(normal);
		}

		//�擪�� mtllib => �}�e���A��
		if (key == "mtllib") {
			string filename;
			line_stream >> filename;
			LoadMatarial(filename, matarialpath);
		}

	}
	//----------�ȍ~�͓ǂݍ��񂾌�̏���----------//

	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	//�ꎞ�I�u�W�F�N�g
	const auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	//���_�o�b�t�@����
	result = NY_Object3DManager::GetDev()->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	//���_�o�b�t�@�r���[�ݒ�
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	RESDESC = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	//�C���f�b�N�X�o�b�t�@����
	result = NY_Object3DManager::GetDev()->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);
	//�C���f�b�N�X�o�b�t�@�r���[�ݒ�
	ibview.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibview.SizeInBytes = sizeIB;
	ibview.Format = DXGI_FORMAT_R16_UINT;

	//���_�o�b�t�@�f�[�^�]��
	Vertex *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	copy(vertices.begin(), vertices.end(), vertMap);
	vertBuff->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�f�[�^�]��
	unsigned short *indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void **)&indexMap);
	copy(indices.begin(), indices.end(), indexMap);
	indexBuff->Unmap(0, nullptr);

}

void NY_Model3D::LoadMatarial(string fileName, string filepath)
{
	ifstream file;
	//�}�e���A���t�@�C���I�[�v��
	file.open(filepath + fileName);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	//1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		//�X�g���[���ϊ�
		istringstream line_stream(line);
		//���p�X�y�[�X��؂�Ő擪������擾
		string key;
		getline(line_stream, key, ' ');
		//�擪�^�u�����͖���
		if (key[0] == '\t') {
			key.erase(key.begin());
		}

		//newmtl == �}�e���A����
		if (key == "newmtl") {
			line_stream >> material.mtlName;
		}

		//Ka == �A���r�G���g
		if (key == "Ka") {
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}

		//Kd == �f�B�t���[�Y
		if (key == "Kd") {
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}

		//Ks == �X�y�L�����[
		if (key == "Ks") {
			line_stream >> material.specurar.x;
			line_stream >> material.specurar.y;
			line_stream >> material.specurar.z;
		}

		//map_Kd == �e�N�X�`���t�@�C����
		if (key == "map_Kd") {
			line_stream >> material.texFileName;
			//�e�N�X�`���̃��[�h
			material.texNumber = TexManager::LoadTexture(filepath + material.texFileName);
		}
	}

}

void NY_Model3D::CreatePlaneModelXY(float x_size, float y_size, float uv_x, float uv_y, UINT useTexNum)
{
	//���_�f�[�^�쐬
	Vertex plane[4] = {
		{{-x_size,-y_size,0},{},{0.0f,0.0f}},
		{{ x_size,-y_size,0},{},{uv_x,0.0f}},
		{{-x_size, y_size,0},{},{0.0f,uv_y}},
		{{ x_size, y_size,0},{},{uv_x,uv_y}},
	};
	//���_�f�[�^�i�[
	for (int i = 0; i < 4; i++)
	{
		vertices.push_back(plane[i]);
	}
	//�C���f�b�N�X�f�[�^�쐬
	unsigned short index[] = {
		0,2,1,
		1,2,3,
	};
	//�C���f�b�N�X�f�[�^�i�[
	for (int i = 0; i < _countof(index); i++)
	{
		indices.push_back(index[i]);
	}

	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	const auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	//���_�o�b�t�@����
	result = NY_Object3DManager::GetDev()->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	//���_�o�b�t�@�r���[�ݒ�
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	RESDESC = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	//�C���f�b�N�X�o�b�t�@����
	result = NY_Object3DManager::GetDev()->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);
	//�C���f�b�N�X�o�b�t�@�r���[�ݒ�
	ibview.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibview.SizeInBytes = sizeIB;
	ibview.Format = DXGI_FORMAT_R16_UINT;

	//���_�o�b�t�@�f�[�^�]��
	Vertex *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	copy(vertices.begin(), vertices.end(), vertMap);
	vertBuff->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�f�[�^�]��
	unsigned short *indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void **)&indexMap);
	copy(indices.begin(), indices.end(), indexMap);
	indexBuff->Unmap(0, nullptr);

	//�}�e���A���̔ԍ��ݒ�
	material.texNumber = useTexNum;

}

void NY_Model3D::CreatePlaneModelXZ(float x_size, float z_size, float uv_x, float uv_y, UINT useTexNum)
{
	//���_�f�[�^�쐬
	Vertex plane[4] = {
		{{-x_size,0,-z_size},{},{0.0f,0.0f}},
		{{ x_size,0,-z_size},{},{uv_x,0.0f}},
		{{-x_size,0, z_size},{},{0.0f,uv_y}},
		{{ x_size,0, z_size},{},{uv_x,uv_y}},
	};
	//���_�f�[�^�i�[
	for (int i = 0; i < 4; i++)
	{
		vertices.push_back(plane[i]);
	}
	//�C���f�b�N�X�f�[�^�쐬
	unsigned short index[] = {
		0,2,1,
		1,2,3,
	};
	//�C���f�b�N�X�f�[�^�i�[
	for (int i = 0; i < _countof(index); i++)
	{
		indices.push_back(index[i]);
	}

	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	const auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	//���_�o�b�t�@����
	result = NY_Object3DManager::GetDev()->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	//���_�o�b�t�@�r���[�ݒ�
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	RESDESC = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	//�C���f�b�N�X�o�b�t�@����
	result = NY_Object3DManager::GetDev()->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);
	//�C���f�b�N�X�o�b�t�@�r���[�ݒ�
	ibview.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibview.SizeInBytes = sizeIB;
	ibview.Format = DXGI_FORMAT_R16_UINT;

	//���_�o�b�t�@�f�[�^�]��
	Vertex *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	copy(vertices.begin(), vertices.end(), vertMap);
	vertBuff->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�f�[�^�]��
	unsigned short *indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void **)&indexMap);
	copy(indices.begin(), indices.end(), indexMap);
	indexBuff->Unmap(0, nullptr);

	//�}�e���A���̔ԍ��ݒ�
	material.texNumber = useTexNum;
}

void NY_Model3D::CreatePlaneModelYZ(float y_size, float z_size, float uv_x, float uv_y, UINT useTexNum)
{
	//���_�f�[�^�쐬
	Vertex plane[4] = {
		{{ 0,-y_size,-z_size},{},{0.0f,0.0f}},
		{{ 0, y_size,-z_size},{},{uv_x,0.0f}},
		{{ 0,-y_size, z_size},{},{0.0f,uv_y}},
		{{ 0, y_size, z_size},{},{uv_x,uv_y}},
	};
	//���_�f�[�^�i�[
	for (int i = 0; i < 4; i++)
	{
		vertices.push_back(plane[i]);
	}
	//�C���f�b�N�X�f�[�^�쐬
	unsigned short index[] = {
		0,2,1,
		1,2,3,
	};
	//�C���f�b�N�X�f�[�^�i�[
	for (int i = 0; i < _countof(index); i++)
	{
		indices.push_back(index[i]);
	}

	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	const auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	//���_�o�b�t�@����
	result = NY_Object3DManager::GetDev()->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	//���_�o�b�t�@�r���[�ݒ�
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	RESDESC = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	//�C���f�b�N�X�o�b�t�@����
	result = NY_Object3DManager::GetDev()->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);
	//�C���f�b�N�X�o�b�t�@�r���[�ݒ�
	ibview.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibview.SizeInBytes = sizeIB;
	ibview.Format = DXGI_FORMAT_R16_UINT;

	//���_�o�b�t�@�f�[�^�]��
	Vertex *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	copy(vertices.begin(), vertices.end(), vertMap);
	vertBuff->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�f�[�^�]��
	unsigned short *indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void **)&indexMap);
	copy(indices.begin(), indices.end(), indexMap);
	indexBuff->Unmap(0, nullptr);

	//�}�e���A���̔ԍ��ݒ�
	material.texNumber = useTexNum;
}

void NY_Model3D::CreateBoxModel(float size, float uv_x, float uv_y, UINT useTexNum)
{

	//���_�f�[�^�쐬
	Vertex box[] = {
		//��
		{{-size,-size,-size},{0,0,1}, {0.0f,1.0f}},//����
		{{-size, size,-size},{0,0,1}, {0.0f,0.0f}},//����
		{{ size,-size,-size},{0,0,1}, {1.0f,1.0f}},//�E��
		{{ size, size,-size},{0,0,1}, {1.0f,0.0f}},//�E��
		//�O
		{{-size,-size, size},{0,0,-1},{0.0f,1.0f}},//����
		{{-size, size, size},{0,0,-1},{0.0f,0.0f}},//����
		{{ size,-size, size},{0,0,-1},{1.0f,1.0f}},//�E��
		{{ size, size, size},{0,0,-1},{1.0f,0.0f}},//�E��
		//��
		{{-size,-size,-size},{-1,0,0},{0.0f,1.0f}},//����
		{{-size,-size, size},{-1,0,0},{0.0f,0.0f}},//����
		{{-size, size,-size},{-1,0,0},{1.0f,1.0f}},//�E��
		{{-size, size, size},{-1,0,0},{1.0f,0.0f}},//�E��
		//�E
		{{ size,-size,-size},{1,0,0},{0.0f,1.0f}},//����
		{{ size,-size, size},{1,0,0},{0.0f,0.0f}},//����
		{{ size, size,-size},{1,0,0},{1.0f,1.0f}},//�E��
		{{ size, size, size},{1,0,0},{1.0f,0.0f}},//�E��
		//��
		{{-size, size,-size},{0,1,0},{1.0f,1.0f}},//�E��
		{{-size, size, size},{0,1,0},{1.0f,0.0f}},//�E��
		{{ size, size,-size},{0,1,0},{0.0f,1.0f}},//����
		{{ size, size, size},{0,1,0},{0.0f,0.0f}},//����
		//��
		{{ size,-size,-size},{0,-1,0},{0.0f,1.0f}},//����
		{{ size,-size, size},{0,-1,0},{0.0f,0.0f}},//����
		{{-size,-size,-size},{0,-1,0},{1.0f,1.0f}},//�E��
		{{-size,-size, size},{0,-1,0},{1.0f,0.0f}},//�E��

	};
	//���_�f�[�^�i�[
	for (int i = 0; i < _countof(box); i++)
	{
		vertices.push_back(box[i]);
	}
	//�C���f�b�N�X�f�[�^�쐬
	unsigned short index[] = {
		//�O
		0,1,2,
		2,1,3,
		//��
		7,5,6,
		6,5,4,
		//��
		8,9,10,
		10,9,11,
		//�E
		14,15,12,
		12,15,13,
		//��
		16,17,18,
		18,17,19,
		//��
		20,21,22,
		22,21,23,
	};
	//�C���f�b�N�X�f�[�^�i�[
	for (int i = 0; i < _countof(index); i++)
	{
		indices.push_back(index[i]);
	}

	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	const auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	//���_�o�b�t�@����
	result = NY_Object3DManager::GetDev()->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	//���_�o�b�t�@�r���[�ݒ�
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	RESDESC = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	//�C���f�b�N�X�o�b�t�@����
	result = NY_Object3DManager::GetDev()->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);
	//�C���f�b�N�X�o�b�t�@�r���[�ݒ�
	ibview.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibview.SizeInBytes = sizeIB;
	ibview.Format = DXGI_FORMAT_R16_UINT;

	//���_�o�b�t�@�f�[�^�]��
	Vertex *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	copy(vertices.begin(), vertices.end(), vertMap);
	vertBuff->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�f�[�^�]��
	unsigned short *indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void **)&indexMap);
	copy(indices.begin(), indices.end(), indexMap);
	indexBuff->Unmap(0, nullptr);

	//�}�e���A���̔ԍ��ݒ�
	material.texNumber = useTexNum;
}

void NY_Model3D::ChangeTexAnimationNumber(int useAnimNum)
{
	//�͈͊O�Q�Ƃ����m
	if (useAnimNum < 0) { return; }
	if (useAnimNum > TexManager::textureData[material.texNumber].uv_offsets.size()) { return; }

	int vposNumber = 0;
	//UV�l���Y���I�t�Z�b�g�̒l�ɕύX
	for (int i = 0; i < vertices.size(); i++) {



		vposNumber++;
		if (vposNumber >= 3) {
			vposNumber == 0;
		}
	}


}
