#include "NY_Object3D.h"
#include "TexManager.h"

#include "Raki_DX12B.h"

//----- NY_Object3D -----//

void Object3d::InitObject3D(Object3d *obj, ID3D12Device *dev)
{
	HRESULT result;
	const auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	const auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);

	//定数バッファb0生成
	result = dev->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&obj->constBuffB0)
	);

	//定数バッファb1生成
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
	//親にヌルを代入
	parent = nullptr;

	HRESULT result;
	const auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	const auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);
	
	//定数バッファb0生成
	result =  dev->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0)
	);

	//定数バッファb1生成
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

	// スケール、回転、平行行列の計算

	matScale = XMMatrixScaling(obj->scale.x, obj->scale.y, obj->scale.z);

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(obj->rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(obj->rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(obj->rotation.y));

	matTrans = XMMatrixTranslation(obj->position.x, obj->position.y, obj->position.z);

	obj->matWorld = XMMatrixIdentity();
	obj->matWorld *= matScale;//ワールド行列にスケーリングを反映
	obj->matWorld *= matRot;
	obj->matWorld *= matTrans;

	if (obj->parent != nullptr)
	{
		obj->matWorld *= obj->parent->matWorld;
	}

	//定数バッファB0データ転送
	ConstBufferDataB0 *ConstMapB0 = nullptr;
	if (SUCCEEDED(obj->constBuffB0->Map(0, nullptr, (void **)&ConstMapB0)))
	{
		ConstMapB0->mat = obj->matWorld * matview * NY_Object3DManager::Get()->matProjection;
		obj->constBuffB0->Unmap(0, nullptr);
	}

	//定数バッファB1データ転送
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
	//ダーティフラグの挙動は検討中
	isDirty = true;

	//ダーティフラグが有効な時更新（通常オブジェクト）
	if (isDirty && isBillBoard != true) {

		XMMATRIX matScale, matRot, matTrans;

		// スケール、回転、平行行列の計算

		matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

		matTrans = XMMatrixTranslation(position.x, position.y, position.z);

		matWorld = XMMatrixIdentity();
		matWorld *= matScale;//ワールド行列にスケーリングを反映
		matWorld *= matRot;
		matWorld *= matTrans;

		if (parent != nullptr)
		{
			matWorld *= parent->matWorld;
		}

		//定数バッファB0データ転送
		ConstBufferDataB0 *ConstMapB0 = nullptr;
		if (SUCCEEDED(constBuffB0->Map(0, nullptr, (void **)&ConstMapB0)))
		{
			ConstMapB0->mat = matWorld * matview->_matView * NY_Object3DManager::Get()->matProjection;
			ConstMapB0->color = this->color;
			constBuffB0->Unmap(0, nullptr);
		}

		//定数バッファB1データ転送
		ConstBufferDataB1 *ConstMapB1 = nullptr;
		if (SUCCEEDED(constBuffB1->Map(0, nullptr, (void **)&ConstMapB1)))
		{
			ConstMapB1->amdient = model->material.ambient;
			ConstMapB1->diffuse = model->material.diffuse;
			ConstMapB1->specular = model->material.specurar;
			ConstMapB1->alpha = model->material.alpha;
			constBuffB1->Unmap(0, nullptr);
		}

		//ダーティフラグリセット
		isDirty = false;
	}
	//ビルボードのとき
	else if (isDirty && isBillBoard) {
		UpdateBillBoard3D(matview);

		isDirty = false;
	}
}

void Object3d::UpdateBillBoard3D(Object3d *obj, NY_Camera cam)
{
	/*
	ビルボードの原理は、ビューで回す方向の反対を予めかけりゃええやん！だが
	NY_Cameraクラスの欠点として、Camera本体のワールド座標は一切替わらないので
	NY_Cameraのインスタンスでビルボードの更新は出来ない。
	よって、Cameraの「追従するターゲット」を用いて逆回転を求める必要がある?
	*/


	////ビルボード用変換行列を宣言
	XMMATRIX billBoardMat;
	billBoardMat = XMMatrixIdentity();

	billBoardMat = XMMatrixLookAtLH({ 0.0f,0.0f,0.0f }, -XMVector3Normalize(XMLoadFloat3(&cam._eye) - XMLoadFloat3(&cam._target)), XMLoadFloat3(&cam._up));
	billBoardMat = XMMatrixInverse(nullptr, billBoardMat);

	//ワールド変換用
	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行行列の計算

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

	//定数バッファB0データ転送
	ConstBufferDataB0 *ConstMapB0 = nullptr;
	if (SUCCEEDED(obj->constBuffB0->Map(0, nullptr, (void **)&ConstMapB0)))
	{
		ConstMapB0->mat = obj->matWorld * cam._matView * NY_Object3DManager::Get()->matProjection;

		obj->constBuffB0->Unmap(0, nullptr);
	}

	//定数バッファB1データ転送
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
	ビルボードの原理は、ビューで回す方向の反対を予めかけりゃええやん！だが
	NY_Cameraクラスの欠点として、Camera本体のワールド座標は一切替わらないので
	NY_Cameraのインスタンスでビルボードの更新は出来ない。
	よって、Cameraの「追従するターゲット」を用いて逆回転を求める必要がある?
	*/


////ビルボード用変換行列を宣言
	XMMATRIX billBoardMat;
	billBoardMat = XMMatrixIdentity();

	billBoardMat = XMMatrixLookAtLH({ 0.0f,0.0f,0.0f }, -XMVector3Normalize(XMLoadFloat3(&cam->_eye) - XMLoadFloat3(&cam->_target)), XMLoadFloat3(&cam->_up));
	billBoardMat = XMMatrixInverse(nullptr, billBoardMat);

	//ワールド変換用
	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行行列の計算

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

	//定数バッファB0データ転送
	ConstBufferDataB0 *ConstMapB0 = nullptr;
	if (SUCCEEDED(constBuffB0->Map(0, nullptr, (void **)&ConstMapB0)))
	{
		ConstMapB0->mat = matWorld * cam->_matView * NY_Object3DManager::Get()->matProjection;
		ConstMapB0->color = this->color;
		constBuffB0->Unmap(0, nullptr);
	}

	//定数バッファB1データ転送
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
	//頂点バッファ設定
	cmd->IASetVertexBuffers(0, 1, &obj->model->vbView);
	//インデックスバッファ設定
	cmd->IASetIndexBuffer(&obj->model->ibview);
	//定数バッファ設定
	cmd->SetGraphicsRootConstantBufferView(0, obj->constBuffB0->GetGPUVirtualAddress());
	//定数バッファ設定
	cmd->SetGraphicsRootConstantBufferView(1, obj->constBuffB1->GetGPUVirtualAddress());

	//シェーダーリソースビューをセット
	cmd->SetGraphicsRootDescriptorTable(2,
	CD3DX12_GPU_DESCRIPTOR_HANDLE(NY_Object3DManager::Get()->descheap->GetGPUDescriptorHandleForHeapStart(),
	obj->model->material.texNumber, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//描画
	cmd->DrawIndexedInstanced(obj->model->indices.size(), 1, 0, 0, 0);
}

void Object3d::DrawModel3D(ID3D12GraphicsCommandList *cmd, ID3D12Device *dev)
{
	//頂点バッファ設定
	cmd->IASetVertexBuffers(0, 1, &model->vbView);
	//インデックスバッファ設定
	cmd->IASetIndexBuffer(&model->ibview);
	//定数バッファ設定
	cmd->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	//定数バッファ設定
	cmd->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	//シェーダーリソースビューをセット
	cmd->SetGraphicsRootDescriptorTable(2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(TexManager::texDsvHeap.Get()->GetGPUDescriptorHandleForHeapStart(),
			model->material.texNumber, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//描画
	cmd->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
}

