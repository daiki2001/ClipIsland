#include "NY_Object3D.h"
#include "TexManager.h"

//----- NY_Model3D -----//

//void NY_Model3D::LoadObjModel(const char *modelName, ID3D12Device *dev, NY_Object3DManager *mgr)
//{
//	//FileStream
//	ifstream file;
//	/*   ���f�����݂̂Ŏw��\�ɂ��邽�߂̏����i�}�e���A���ǂݍ��݂��y�ɂ��邽�߁j   */
//	string fileName = modelName;
//	string modelpath = "Resources/" + fileName;
//
//	//obj�t�@�C���I�[�v��
//	file.open(modelpath + ".obj");
//	//�I�[�v�����s���`�F�b�N
//	if (file.fail())
//	{
//		assert(0);
//	}
//
//	//���l�i�[
//	vector<XMFLOAT3> positions;
//	vector<XMFLOAT3> normals;
//	vector<XMFLOAT2> texcoords;
//
//	//1�s���ǂݍ���
//	string line;
//	while (getline(file, line)) {
//
//		//1�s���̕�������X�g���[���ɕϊ����ēǂ݂₷������
//		istringstream line_stream(line);
//
//		//���p�X�y�[�X�ōs�̐擪������擾
//		string key;
//		getline(line_stream, key, ' ');
//
//		//�擪�� v => ���_���W
//		if (key == "v") {
//			//xyz���W�ǂݍ���
//			XMFLOAT3 loadedPos{};
//			line_stream >> loadedPos.x;
//			line_stream >> loadedPos.y;
//			line_stream >> loadedPos.z;
//			//���W�f�[�^�ǉ�
//			positions.emplace_back(loadedPos);
//		}
//
//		//�擪�� f => �|���S���C���f�b�N�X
//		if (key == "f") {
//			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
//			string index_string;
//			while (getline(line_stream, index_string, ' ')) {
//				//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ēǂ݂₷������
//				istringstream index_stream(index_string);
//				unsigned short indexPosition, indexNormal, indexTexcoord;
//				index_stream >> indexPosition;
//				index_stream.seekg(1, ios_base::cur);//�X���b�V����΂�
//				index_stream >> indexTexcoord;
//				index_stream.seekg(1, ios_base::cur);//�X���b�V����΂�
//				index_stream >> indexNormal;
//				//���_�f�[�^�ǉ�
//				Vertex vertex{};
//				vertex.pos = positions[indexPosition - 1];
//				vertex.normal = normals[indexNormal - 1];
//				vertex.uv = texcoords[indexTexcoord - 1];
//				vertices.emplace_back(vertex);
//				//���_�C���f�b�N�X�ɒǉ�
//				indices.emplace_back((unsigned short)indices.size());
//			}
//		}
//
//		//�擪�� vt => �e�N�X�`��
//		if (key == "vt") {
//			//U,V�����ǂݍ���
//			XMFLOAT2 texcoord{};
//			line_stream >> texcoord.x;
//			line_stream >> texcoord.y;
//			//V�������]
//			texcoord.y = 1.0f - texcoord.y;
//			//�e�N�X�`�����W�f�[�^�ɒǉ�
//			texcoords.emplace_back(texcoord);
//		}
//
//		//�擪�� vn => �@���x�N�g��
//		if (key == "vn") {
//			//x,y,z�����ǂݍ���
//			XMFLOAT3 normal{};
//			line_stream >> normal.x;
//			line_stream >> normal.y;
//			line_stream >> normal.z;
//			//�@���x�N�g���Ƀf�[�^�ǉ�
//			normals.emplace_back(normal);
//		}
//
//		//�擪�� mtllib => �}�e���A��
//		if (key == "mtllib") {
//			string filename;
//			line_stream >> filename;
//			LoadMatarial(filename, dev, mgr);
//		}
//
//	}
//	//----------�ȍ~�͓ǂݍ��񂾌�̏���----------//
//
//	HRESULT result;
//
//	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
//	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
//
//	//���_�o�b�t�@����
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&vertBuff)
//	);
//	//���_�o�b�t�@�r���[�ݒ�
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	vbView.SizeInBytes = sizeVB;
//	vbView.StrideInBytes = sizeof(Vertex);
//
//	//�C���f�b�N�X�o�b�t�@����
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&indexBuff)
//	);
//	//�C���f�b�N�X�o�b�t�@�r���[�ݒ�
//	ibview.BufferLocation = indexBuff->GetGPUVirtualAddress();
//	ibview.SizeInBytes = sizeIB;
//	ibview.Format = DXGI_FORMAT_R16_UINT;
//
//	//���_�o�b�t�@�f�[�^�]��
//	Vertex *vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
//	copy(vertices.begin(), vertices.end(), vertMap);
//	vertBuff->Unmap(0, nullptr);
//
//	//�C���f�b�N�X�o�b�t�@�f�[�^�]��
//	unsigned short *indexMap = nullptr;
//	result = indexBuff->Map(0, nullptr, (void **)&indexMap);
//	copy(indices.begin(), indices.end(), indexMap);
//	indexBuff->Unmap(0, nullptr);
//
//}
//
//void NY_Model3D::LoadMatarial(string path, ID3D12Device *dev, NY_Object3DManager *mgr)
//{
//	ifstream file;
//	//�}�e���A���t�@�C���I�[�v��
//	file.open("Resources/" + path);
//	//�t�@�C���I�[�v�����s���`�F�b�N
//	if (file.fail()) {
//		assert(0);
//	}
//
//	//1�s���ǂݍ���
//	string line;
//	while (getline(file, line))
//	{
//		//�X�g���[���ϊ�
//		istringstream line_stream(line);
//		//���p�X�y�[�X��؂�Ő擪������擾
//		string key;
//		getline(line_stream, key, ' ');
//		//�擪�^�u�����͖���
//		if (key[0] == '\t') {
//			key.erase(key.begin());
//		}
//
//		//newmtl == �}�e���A����
//		if (key == "newmtl") {
//			line_stream >> material.mtlName;
//		}
//
//		//Ka == �A���r�G���g
//		if (key == "Ka") {
//			line_stream >> material.ambient.x;
//			line_stream >> material.ambient.y;
//			line_stream >> material.ambient.z;
//		}
//
//		//Kd == �f�B�t���[�Y
//		if (key == "Kd") {
//			line_stream >> material.diffuse.x;
//			line_stream >> material.diffuse.y;
//			line_stream >> material.diffuse.z;
//		}
//
//		//Ks == �X�y�L�����[
//		if (key == "Ks") {
//			line_stream >> material.specurar.x;
//			line_stream >> material.specurar.y;
//			line_stream >> material.specurar.z;
//		}
//
//		//map_Kd == �e�N�X�`���t�@�C����
//		if (key == "map_Kd") {
//			line_stream >> material.texFileName;
//			//�e�N�X�`�����i�[����Ă��Ȃ��e�N�X�`���o�b�t�@��T��
//			NY_Object3DManager::Get()->LoadObject3DTexture(material.texNumber, material.texFileName, dev);
//		}
//	}
//
//}
//
////void NY_Model3D::CreatePlaneModelXY(float x_size, float y_size, string useTexFileName, ID3D12Device *dev, NY_Object3DManager *mgr)
////{
////	//���_�f�[�^�쐬
////	Vertex plane[4] = {
////		{{-x_size,-y_size,0},{},{0.0f,0.0f}},
////		{{ x_size,-y_size,0},{},{1.0f,0.0f}},
////		{{-x_size, y_size,0},{},{0.0f,1.0f}},
////		{{ x_size, y_size,0},{},{1.0f,1.0f}},
////	};
////	//���_�f�[�^�i�[
////	for (int i = 0; i < 4; i++)
////	{
////		vertices.push_back(plane[i]);
////	}
////	//�C���f�b�N�X�f�[�^�쐬
////	unsigned short index[] = {
////		0,1,2,
////		2,1,3,
////	};
////	//�C���f�b�N�X�f�[�^�i�[
////	for (int i = 0; i < _countof(index); i++)
////	{
////		indices.push_back(index[i]);
////	}
////
////	HRESULT result;
////
////	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
////	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
////
////	//���_�o�b�t�@����
////	result = dev->CreateCommittedResource(
////		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
////		D3D12_HEAP_FLAG_NONE,
////		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
////		D3D12_RESOURCE_STATE_GENERIC_READ,
////		nullptr,
////		IID_PPV_ARGS(&vertBuff)
////	);
////	//���_�o�b�t�@�r���[�ݒ�
////	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
////	vbView.SizeInBytes = sizeVB;
////	vbView.StrideInBytes = sizeof(Vertex);
////
////	//�C���f�b�N�X�o�b�t�@����
////	result = dev->CreateCommittedResource(
////		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
////		D3D12_HEAP_FLAG_NONE,
////		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
////		D3D12_RESOURCE_STATE_GENERIC_READ,
////		nullptr,
////		IID_PPV_ARGS(&indexBuff)
////	);
////	//�C���f�b�N�X�o�b�t�@�r���[�ݒ�
////	ibview.BufferLocation = indexBuff->GetGPUVirtualAddress();
////	ibview.SizeInBytes = sizeIB;
////	ibview.Format = DXGI_FORMAT_R16_UINT;
////
////	//���_�o�b�t�@�f�[�^�]��
////	Vertex *vertMap = nullptr;
////	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
////	copy(vertices.begin(), vertices.end(), vertMap);
////	vertBuff->Unmap(0, nullptr);
////
////	//�C���f�b�N�X�o�b�t�@�f�[�^�]��
////	unsigned short *indexMap = nullptr;
////	result = indexBuff->Map(0, nullptr, (void **)&indexMap);
////	copy(indices.begin(), indices.end(), indexMap);
////	indexBuff->Unmap(0, nullptr);
////
////	//�e�N�X�`�����[�h
////	//�e�N�X�`�����i�[����Ă��Ȃ��e�N�X�`���o�b�t�@��T��
////	for (int texpath = 1; texpath < _countof(mgr->texbuff); texpath++)
////	{
////		if (mgr->texbuff[texpath] == nullptr)//��̃e�N�X�`���o�b�t�@�𔭌�
////		{
////			//Obj3DMgr�̃e�N�X�`���ǂݍ��݊֐������s
////			mgr->LoadObject3DTexture((UINT)texpath, useTexFileName, dev);
////			//texNumber�Ƀ}�e���A���Ń��[�h�����e�N�X�`����SRV�ԍ����i�[
////			material.texNumber = texpath;
////			//���[�h�����烋�[�v�𔲂���
////			break;
////		}
////	}
////
////}
//
//void NY_Model3D::CreatePlaneModelXY(float x_size, float y_size, float uv_x, float uv_y, UINT useTexNum, ID3D12Device *dev)
//{
//	//���_�f�[�^�쐬
//	Vertex plane[4] = {
//		{{-x_size,-y_size,0},{},{0.0f,0.0f}},
//		{{ x_size,-y_size,0},{},{uv_x,0.0f}},
//		{{-x_size, y_size,0},{},{0.0f,uv_y}},
//		{{ x_size, y_size,0},{},{uv_x,uv_y}},
//	};
//	//���_�f�[�^�i�[
//	for (int i = 0; i < 4; i++)
//	{
//		vertices.push_back(plane[i]);
//	}
//	//�C���f�b�N�X�f�[�^�쐬
//	unsigned short index[] = {
//		0,2,1,
//		1,2,3,
//	};
//	//�C���f�b�N�X�f�[�^�i�[
//	for (int i = 0; i < _countof(index); i++)
//	{
//		indices.push_back(index[i]);
//	}
//
//	HRESULT result;
//
//	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
//	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
//
//	//���_�o�b�t�@����
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&vertBuff)
//	);
//	//���_�o�b�t�@�r���[�ݒ�
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	vbView.SizeInBytes = sizeVB;
//	vbView.StrideInBytes = sizeof(Vertex);
//
//	//�C���f�b�N�X�o�b�t�@����
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&indexBuff)
//	);
//	//�C���f�b�N�X�o�b�t�@�r���[�ݒ�
//	ibview.BufferLocation = indexBuff->GetGPUVirtualAddress();
//	ibview.SizeInBytes = sizeIB;
//	ibview.Format = DXGI_FORMAT_R16_UINT;
//
//	//���_�o�b�t�@�f�[�^�]��
//	Vertex *vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
//	copy(vertices.begin(), vertices.end(), vertMap);
//	vertBuff->Unmap(0, nullptr);
//
//	//�C���f�b�N�X�o�b�t�@�f�[�^�]��
//	unsigned short *indexMap = nullptr;
//	result = indexBuff->Map(0, nullptr, (void **)&indexMap);
//	copy(indices.begin(), indices.end(), indexMap);
//	indexBuff->Unmap(0, nullptr);
//
//	//�}�e���A���̔ԍ��ݒ�
//	material.texNumber = useTexNum;
//
//}
//
//void NY_Model3D::CreatePlaneModelXZ(float x_size, float z_size, float uv_x, float uv_y, UINT useTexNum, ID3D12Device *dev)
//{
//	//���_�f�[�^�쐬
//	Vertex plane[4] = {
//		{{-x_size,0,-z_size},{},{0.0f,0.0f}},
//		{{ x_size,0,-z_size},{},{uv_x,0.0f}},
//		{{-x_size,0, z_size},{},{0.0f,uv_y}},
//		{{ x_size,0, z_size},{},{uv_x,uv_y}},
//	};
//	//���_�f�[�^�i�[
//	for (int i = 0; i < 4; i++)
//	{
//		vertices.push_back(plane[i]);
//	}
//	//�C���f�b�N�X�f�[�^�쐬
//	unsigned short index[] = {
//		0,2,1,
//		1,2,3,
//	};
//	//�C���f�b�N�X�f�[�^�i�[
//	for (int i = 0; i < _countof(index); i++)
//	{
//		indices.push_back(index[i]);
//	}
//
//	HRESULT result;
//
//	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
//	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
//
//	//���_�o�b�t�@����
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&vertBuff)
//	);
//	//���_�o�b�t�@�r���[�ݒ�
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	vbView.SizeInBytes = sizeVB;
//	vbView.StrideInBytes = sizeof(Vertex);
//
//	//�C���f�b�N�X�o�b�t�@����
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&indexBuff)
//	);
//	//�C���f�b�N�X�o�b�t�@�r���[�ݒ�
//	ibview.BufferLocation = indexBuff->GetGPUVirtualAddress();
//	ibview.SizeInBytes = sizeIB;
//	ibview.Format = DXGI_FORMAT_R16_UINT;
//
//	//���_�o�b�t�@�f�[�^�]��
//	Vertex *vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
//	copy(vertices.begin(), vertices.end(), vertMap);
//	vertBuff->Unmap(0, nullptr);
//
//	//�C���f�b�N�X�o�b�t�@�f�[�^�]��
//	unsigned short *indexMap = nullptr;
//	result = indexBuff->Map(0, nullptr, (void **)&indexMap);
//	copy(indices.begin(), indices.end(), indexMap);
//	indexBuff->Unmap(0, nullptr);
//
//	//�}�e���A���̔ԍ��ݒ�
//	material.texNumber = useTexNum;
//}
//
//void NY_Model3D::CreatePlaneModelYZ(float y_size, float z_size, float uv_x, float uv_y, UINT useTexNum, ID3D12Device *dev)
//{
//	//���_�f�[�^�쐬
//	Vertex plane[4] = {
//		{{ 0,-y_size,-z_size},{},{0.0f,0.0f}},
//		{{ 0, y_size,-z_size},{},{uv_x,0.0f}},
//		{{ 0,-y_size, z_size},{},{0.0f,uv_y}},
//		{{ 0, y_size, z_size},{},{uv_x,uv_y}},
//	};
//	//���_�f�[�^�i�[
//	for (int i = 0; i < 4; i++)
//	{
//		vertices.push_back(plane[i]);
//	}
//	//�C���f�b�N�X�f�[�^�쐬
//	unsigned short index[] = {
//		0,2,1,
//		1,2,3,
//	};
//	//�C���f�b�N�X�f�[�^�i�[
//	for (int i = 0; i < _countof(index); i++)
//	{
//		indices.push_back(index[i]);
//	}
//
//	HRESULT result;
//
//	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
//	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
//
//	//���_�o�b�t�@����
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&vertBuff)
//	);
//	//���_�o�b�t�@�r���[�ݒ�
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	vbView.SizeInBytes = sizeVB;
//	vbView.StrideInBytes = sizeof(Vertex);
//
//	//�C���f�b�N�X�o�b�t�@����
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&indexBuff)
//	);
//	//�C���f�b�N�X�o�b�t�@�r���[�ݒ�
//	ibview.BufferLocation = indexBuff->GetGPUVirtualAddress();
//	ibview.SizeInBytes = sizeIB;
//	ibview.Format = DXGI_FORMAT_R16_UINT;
//
//	//���_�o�b�t�@�f�[�^�]��
//	Vertex *vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
//	copy(vertices.begin(), vertices.end(), vertMap);
//	vertBuff->Unmap(0, nullptr);
//
//	//�C���f�b�N�X�o�b�t�@�f�[�^�]��
//	unsigned short *indexMap = nullptr;
//	result = indexBuff->Map(0, nullptr, (void **)&indexMap);
//	copy(indices.begin(), indices.end(), indexMap);
//	indexBuff->Unmap(0, nullptr);
//
//	//�}�e���A���̔ԍ��ݒ�
//	material.texNumber = useTexNum;
//}
//
//void NY_Model3D::CreateBoxModel(float size, float uv_x, float uv_y, UINT useTexNum, ID3D12Device *dev)
//{
//	//���_�f�[�^�쐬
//	Vertex box[] = {
//		//��
//		{{-size,-size,-size},{}, {0.0f,1.0f}},//����
//		{{-size, size,-size},{}, {0.0f,0.0f}},//����
//		{{ size,-size,-size},{}, {1.0f,1.0f}},//�E��
//		{{ size, size,-size},{}, {1.0f,0.0f}},//�E��
//		//�O
//		{{-size,-size, size},{},{0.0f,1.0f}},//����
//		{{-size, size, size},{},{0.0f,0.0f}},//����
//		{{ size,-size, size},{},{1.0f,1.0f}},//�E��
//		{{ size, size, size},{},{1.0f,0.0f}},//�E��
//		//��
//		{{-size,-size,-size},{},{0.0f,1.0f}},//����
//		{{-size,-size, size},{},{0.0f,0.0f}},//����
//		{{-size, size,-size},{},{1.0f,1.0f}},//�E��
//		{{-size, size, size},{},{1.0f,0.0f}},//�E��
//		//�E
//		{{ size,-size,-size},{},{0.0f,1.0f}},//����
//		{{ size,-size, size},{},{0.0f,0.0f}},//����
//		{{ size, size,-size},{},{1.0f,1.0f}},//�E��
//		{{ size, size, size},{},{1.0f,0.0f}},//�E��
//		//��
//		{{-size, size,-size},{},{1.0f,1.0f}},//�E��
//		{{-size, size, size},{},{1.0f,0.0f}},//�E��
//		{{ size, size,-size},{},{0.0f,1.0f}},//����
//		{{ size, size, size},{},{0.0f,0.0f}},//����
//		//��
//		{{ size,-size,-size},{},{0.0f,1.0f}},//����
//		{{ size,-size, size},{},{0.0f,0.0f}},//����
//		{{-size,-size,-size},{},{1.0f,1.0f}},//�E��
//		{{-size,-size, size},{},{1.0f,0.0f}},//�E��
//
//	};
//	//���_�f�[�^�i�[
//	for (int i = 0; i < _countof(box); i++)
//	{
//		vertices.push_back(box[i]);
//	}
//	//�C���f�b�N�X�f�[�^�쐬
//	unsigned short index[] = {
//		//�O
//		0,1,2,
//		2,1,3,
//		//��
//		7,5,6,
//		6,5,4,
//		//��
//		8,9,10,
//		10,9,11,
//		//�E
//		14,15,12,
//		12,15,13,
//		//��
//		16,17,18,
//		18,17,19,
//		//��
//		20,21,22,
//		22,21,23,
//	};
//	//�C���f�b�N�X�f�[�^�i�[
//	for (int i = 0; i < _countof(index); i++)
//	{
//		indices.push_back(index[i]);
//	}
//
//	HRESULT result;
//
//	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
//	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
//
//	//���_�o�b�t�@����
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&vertBuff)
//	);
//	//���_�o�b�t�@�r���[�ݒ�
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	vbView.SizeInBytes = sizeVB;
//	vbView.StrideInBytes = sizeof(Vertex);
//
//	//�C���f�b�N�X�o�b�t�@����
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&indexBuff)
//	);
//	//�C���f�b�N�X�o�b�t�@�r���[�ݒ�
//	ibview.BufferLocation = indexBuff->GetGPUVirtualAddress();
//	ibview.SizeInBytes = sizeIB;
//	ibview.Format = DXGI_FORMAT_R16_UINT;
//
//	//���_�o�b�t�@�f�[�^�]��
//	Vertex *vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
//	copy(vertices.begin(), vertices.end(), vertMap);
//	vertBuff->Unmap(0, nullptr);
//
//	//�C���f�b�N�X�o�b�t�@�f�[�^�]��
//	unsigned short *indexMap = nullptr;
//	result = indexBuff->Map(0, nullptr, (void **)&indexMap);
//	copy(indices.begin(), indices.end(), indexMap);
//	indexBuff->Unmap(0, nullptr);
//
//	//�}�e���A���̔ԍ��ݒ�
//	material.texNumber = useTexNum;
//}
//
//
//
//void NY_Model3D::CreateTriangleGeometoryTest(UINT usetexnum,ID3D12Device *dev)
//{
//	//���_�f�[�^�쐬
//	ParticleVertex plane[] = {
//		{{0.0f,0.0f,0.0f}},
//	};
//	//���_�f�[�^�i�[
//	for (int i = 0; i < 4; i++)
//	{
//		pvert.push_back(plane[i]);
//	}
//
//	HRESULT result;
//
//	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * pvert.size());
//
//	//���_�o�b�t�@����
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&vertBuff)
//	);
//
//	//���_�o�b�t�@�r���[�ݒ�
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	vbView.SizeInBytes = sizeVB;
//	vbView.StrideInBytes = sizeof(Vertex);
//
//	//���_�o�b�t�@�f�[�^�]��
//	ParticleVertex *vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
//	copy(pvert.begin(), pvert.end(), vertMap);
//	vertBuff->Unmap(0, nullptr);
//
//	//�e�N�X�`�����[�h
//	material.texNumber = usetexnum;
//
//
//}
//
//SingleVertex NY_Model3D::GetSingleVertexData(ID3D12Device *dev)
//{
//	SingleVertex rv;
//
//	rv.v = { {0,0,0},{},{0,0} };
//
//	HRESULT result;
//
//	UINT sizeVB = static_cast<UINT>(sizeof(Vertex));
//
//	//���_�o�b�t�@����
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&rv.vb)
//	);
//
//	//���_�o�b�t�@�r���[�ݒ�
//	rv.vbview.BufferLocation = rv.vb->GetGPUVirtualAddress();
//	rv.vbview.SizeInBytes = sizeVB;
//	rv.vbview.StrideInBytes = sizeof(Vertex);
//
//	//���_�o�b�t�@�f�[�^�]��
//	Vertex *vertMap = nullptr;
//	result = rv.vb -> Map(0, nullptr, (void **)&vertMap);
//	vertMap = &rv.v;
//	rv.vb -> Unmap(0, nullptr);
//
//	return rv;
//}

//----- NY_Object3D -----//

void Object3d::InitObject3D(Object3d *obj, ID3D12Device *dev)
{
	HRESULT result;
	const auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	const auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);

	//�萔�o�b�t�@b0����
	result = dev->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&obj->constBuffB0)
	);

	//�萔�o�b�t�@b1����
	result = dev->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&obj->constBuffB1)
	);

}

void Object3d::InitObject3D(ID3D12Device *dev)
{
	HRESULT result;
	const auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	const auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);

	//�萔�o�b�t�@b0����
	result = dev->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0)
	);

	//�萔�o�b�t�@b1����
	result = dev->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1)
	);
}

void Object3d::SetLoadedModelData(Object3d *obj, NY_Model3D *loadedModel)
{
	obj->model = loadedModel;
}

void Object3d::SetLoadedModelData(NY_Model3D *loadedModel)
{
	model = loadedModel;

	isDirty = true;
}

void Object3d::UpdateObject3D(Object3d *obj, XMMATRIX &matview)
{
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�s��̌v�Z

	matScale = XMMatrixScaling(obj->scale.x, obj->scale.y, obj->scale.z);

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(obj->rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(obj->rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(obj->rotation.y));

	matTrans = XMMatrixTranslation(obj->position.x, obj->position.y, obj->position.z);

	obj->matWorld = XMMatrixIdentity();
	obj->matWorld *= matScale;//���[���h�s��ɃX�P�[�����O�𔽉f
	obj->matWorld *= matRot;
	obj->matWorld *= matTrans;

	if (obj->parent != nullptr)
	{
		obj->matWorld *= obj->parent->matWorld;
	}

	//�萔�o�b�t�@B0�f�[�^�]��
	ConstBufferDataB0 *ConstMapB0 = nullptr;
	if (SUCCEEDED(obj->constBuffB0->Map(0, nullptr, (void **)&ConstMapB0)))
	{
		ConstMapB0->mat = obj->matWorld * matview * NY_Object3DManager::Get()->matProjection;
		obj->constBuffB0->Unmap(0, nullptr);
	}

	//�萔�o�b�t�@B1�f�[�^�]��
	ConstBufferDataB1 *ConstMapB1 = nullptr;
	if (SUCCEEDED(obj->constBuffB1->Map(0, nullptr, (void **)&ConstMapB1)))
	{
		ConstMapB1->amdient  = obj->model->material.ambient;
		ConstMapB1->diffuse  = obj->model->material.diffuse;
		ConstMapB1->specular = obj->model->material.specurar;
		ConstMapB1->alpha    = obj->model->material.alpha;
		obj->constBuffB1->Unmap(0, nullptr);
	}

}

void Object3d::UpdateObject3D(NY_Camera *matview)
{
	//�_�[�e�B�t���O�̋����͌�����
	isDirty = true;

	//�_�[�e�B�t���O���L���Ȏ��X�V�i�ʏ�I�u�W�F�N�g�j
	if (isDirty && isBillBoard != true) {

		XMMATRIX matScale, matRot, matTrans;

		// �X�P�[���A��]�A���s�s��̌v�Z

		matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

		matTrans = XMMatrixTranslation(position.x, position.y, position.z);

		matWorld = XMMatrixIdentity();
		matWorld *= matScale;//���[���h�s��ɃX�P�[�����O�𔽉f
		matWorld *= matRot;
		matWorld *= matTrans;

		if (parent != nullptr)
		{
			matWorld *= parent->matWorld;
		}

		//�萔�o�b�t�@B0�f�[�^�]��
		ConstBufferDataB0 *ConstMapB0 = nullptr;
		if (SUCCEEDED(constBuffB0->Map(0, nullptr, (void **)&ConstMapB0)))
		{
			ConstMapB0->mat = matWorld * matview->_matView * NY_Object3DManager::Get()->matProjection;
			ConstMapB0->color = this->color;
			constBuffB0->Unmap(0, nullptr);
		}

		//�萔�o�b�t�@B1�f�[�^�]��
		ConstBufferDataB1 *ConstMapB1 = nullptr;
		if (SUCCEEDED(constBuffB1->Map(0, nullptr, (void **)&ConstMapB1)))
		{
			ConstMapB1->amdient = model->material.ambient;
			ConstMapB1->diffuse = model->material.diffuse;
			ConstMapB1->specular = model->material.specurar;
			ConstMapB1->alpha = model->material.alpha;
			constBuffB1->Unmap(0, nullptr);
		}

		//�_�[�e�B�t���O���Z�b�g
		isDirty = false;
	}
	//�r���{�[�h�̂Ƃ�
	else if (isDirty && isBillBoard) {
		UpdateBillBoard3D(matview);

		isDirty = false;
	}
}

void Object3d::UpdateBillBoard3D(Object3d *obj, NY_Camera cam)
{
	/*
	�r���{�[�h�̌����́A�r���[�ŉ񂷕����̔��΂�\�߂�����Ⴆ�����I����
	NY_Camera�N���X�̌��_�Ƃ��āACamera�{�̂̃��[���h���W�͈�ؑւ��Ȃ��̂�
	NY_Camera�̃C���X�^���X�Ńr���{�[�h�̍X�V�͏o���Ȃ��B
	����āACamera�́u�Ǐ]����^�[�Q�b�g�v��p���ċt��]�����߂�K�v������?
	*/


	////�r���{�[�h�p�ϊ��s���錾
	XMMATRIX billBoardMat;
	billBoardMat = XMMatrixIdentity();

	billBoardMat = XMMatrixLookAtLH({ 0.0f,0.0f,0.0f }, -XMVector3Normalize(XMLoadFloat3(&cam._eye) - XMLoadFloat3(&cam._target)), XMLoadFloat3(&cam._up));
	billBoardMat = XMMatrixInverse(nullptr, billBoardMat);

	//���[���h�ϊ��p
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�s��̌v�Z

	matScale = XMMatrixScaling(obj->scale.x, obj->scale.y, obj->scale.z);

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(obj->rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(obj->rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(obj->rotation.y));

	matTrans = XMMatrixTranslation(obj->position.x, obj->position.y, obj->position.z);

	obj->matWorld = XMMatrixIdentity();

	obj->matWorld *= billBoardMat;

	obj->matWorld *= matScale;
	obj->matWorld *= matRot;
	obj->matWorld *= matTrans;

	if (obj->parent != nullptr)
	{
		obj->matWorld *= obj->parent->matWorld;
	}

	//�萔�o�b�t�@B0�f�[�^�]��
	ConstBufferDataB0 *ConstMapB0 = nullptr;
	if (SUCCEEDED(obj->constBuffB0->Map(0, nullptr, (void **)&ConstMapB0)))
	{
		ConstMapB0->mat = obj->matWorld * cam._matView * NY_Object3DManager::Get()->matProjection;

		obj->constBuffB0->Unmap(0, nullptr);
	}

	//�萔�o�b�t�@B1�f�[�^�]��
	ConstBufferDataB1 *ConstMapB1 = nullptr;
	if (SUCCEEDED(obj->constBuffB1->Map(0, nullptr, (void **)&ConstMapB1)))
	{
		ConstMapB1->amdient  = obj->model->material.ambient;
		ConstMapB1->diffuse  = obj->model->material.diffuse;
		ConstMapB1->specular = obj->model->material.specurar;
		ConstMapB1->alpha    = obj->model->material.alpha;
		obj->constBuffB1->Unmap(0, nullptr);
	}

}

void Object3d::UpdateBillBoard3D(NY_Camera *cam)
{
	/*
	�r���{�[�h�̌����́A�r���[�ŉ񂷕����̔��΂�\�߂�����Ⴆ�����I����
	NY_Camera�N���X�̌��_�Ƃ��āACamera�{�̂̃��[���h���W�͈�ؑւ��Ȃ��̂�
	NY_Camera�̃C���X�^���X�Ńr���{�[�h�̍X�V�͏o���Ȃ��B
	����āACamera�́u�Ǐ]����^�[�Q�b�g�v��p���ċt��]�����߂�K�v������?
	*/


////�r���{�[�h�p�ϊ��s���錾
	XMMATRIX billBoardMat;
	billBoardMat = XMMatrixIdentity();

	billBoardMat = XMMatrixLookAtLH({ 0.0f,0.0f,0.0f }, -XMVector3Normalize(XMLoadFloat3(&cam->_eye) - XMLoadFloat3(&cam->_target)), XMLoadFloat3(&cam->_up));
	billBoardMat = XMMatrixInverse(nullptr, billBoardMat);

	//���[���h�ϊ��p
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�s��̌v�Z

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();

	matWorld *= billBoardMat;

	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}

	//�萔�o�b�t�@B0�f�[�^�]��
	ConstBufferDataB0 *ConstMapB0 = nullptr;
	if (SUCCEEDED(constBuffB0->Map(0, nullptr, (void **)&ConstMapB0)))
	{
		ConstMapB0->mat = matWorld * cam->_matView * NY_Object3DManager::Get()->matProjection;
		ConstMapB0->color = this->color;
		constBuffB0->Unmap(0, nullptr);
	}

	//�萔�o�b�t�@B1�f�[�^�]��
	ConstBufferDataB1 *ConstMapB1 = nullptr;
	if (SUCCEEDED(constBuffB1->Map(0, nullptr, (void **)&ConstMapB1)))
	{
		ConstMapB1->amdient		= model->material.ambient;
		ConstMapB1->diffuse		= model->material.diffuse;
		ConstMapB1->specular	= model->material.specurar;
		ConstMapB1->alpha		= model->material.alpha;
		constBuffB1->Unmap(0, nullptr);
	}
}

void Object3d::DrawModel3D(Object3d *obj, ID3D12GraphicsCommandList *cmd, ID3D12Device *dev)
{
	//���_�o�b�t�@�ݒ�
	cmd->IASetVertexBuffers(0, 1, &obj->model->vbView);
	//�C���f�b�N�X�o�b�t�@�ݒ�
	cmd->IASetIndexBuffer(&obj->model->ibview);
	//�萔�o�b�t�@�ݒ�
	cmd->SetGraphicsRootConstantBufferView(0, obj->constBuffB0->GetGPUVirtualAddress());
	//�萔�o�b�t�@�ݒ�
	cmd->SetGraphicsRootConstantBufferView(1, obj->constBuffB1->GetGPUVirtualAddress());

	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	cmd->SetGraphicsRootDescriptorTable(2,
	CD3DX12_GPU_DESCRIPTOR_HANDLE(NY_Object3DManager::Get()->descheap->GetGPUDescriptorHandleForHeapStart(),
	obj->model->material.texNumber, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�`��
	cmd->DrawIndexedInstanced(obj->model->indices.size(), 1, 0, 0, 0);
}

void Object3d::DrawModel3D(ID3D12GraphicsCommandList *cmd, ID3D12Device *dev)
{
	//���_�o�b�t�@�ݒ�
	cmd->IASetVertexBuffers(0, 1, &model->vbView);
	//�C���f�b�N�X�o�b�t�@�ݒ�
	cmd->IASetIndexBuffer(&model->ibview);
	//�萔�o�b�t�@�ݒ�
	cmd->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	//�萔�o�b�t�@�ݒ�
	cmd->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	cmd->SetGraphicsRootDescriptorTable(2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(TexManager::texDsvHeap.Get()->GetGPUDescriptorHandleForHeapStart(),
			model->material.texNumber, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�`��
	cmd->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
}

