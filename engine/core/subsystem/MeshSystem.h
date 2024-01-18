//
// (c) 2022 Eduardo Doria.
//

#ifndef MESHSYSTEM_H
#define MESHSYSTEM_H

#include "SubSystem.h"

#include "component/MeshComponent.h"
#include "component/ModelComponent.h"
#include "component/SpriteComponent.h"
#include "component/MeshPolygonComponent.h"
#include "component/CameraComponent.h"
#include "component/TerrainComponent.h"
#include "component/TilemapComponent.h"

namespace cg{

	class MeshSystem : public SubSystem {

    private:
        void createSprite(SpriteComponent& sprite, MeshComponent& mesh, CameraComponent& camera);
		void createMeshPolygon(MeshPolygonComponent& polygon, MeshComponent& mesh);
		void createTilemap(TilemapComponent& tilemap, MeshComponent& mesh);

		void changeFlipY(bool& flipY, CameraComponent& camera, MeshComponent& mesh);
		Rect normalizeTileRect(Rect tileRect, unsigned int texWidth, unsigned int texHeight);

		// Mesh aux
		std::vector<float> getCylinderSideNormals(float baseRadius, float topRadius, float height, float slices);
		std::vector<float> buildUnitCircleVertices(float slices);

		// Model
		static std::string readFileToString(const char* filename);
		static bool fileExists(const std::string &abs_filename, void *);
		static bool readWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filepath, void *);
		void addSubmeshAttribute(Submesh& submesh, std::string bufferName, AttributeType attribute, unsigned int elements, AttributeDataType dataType, size_t size, size_t offset, bool normalized);
		bool loadGLTFBuffer(int bufferViewIndex, MeshComponent& mesh, ModelComponent& model, const int stride, std::vector<std::string>& loadedBuffers);
		bool loadGLTFTexture(int textureIndex, ModelComponent& model, Texture& texture, std::string textureName);
		std::string getBufferName(int bufferViewIndex, ModelComponent& model);
		Matrix4 getGLTFNodeMatrix(int nodeIndex, ModelComponent& model);
		Matrix4 getGLTFMeshGlobalMatrix(int nodeIndex, ModelComponent& model, std::map<int, int>& nodesParent);
		Entity generateSketetalStructure(Entity entity, ModelComponent& model, int nodeIndex, int skinIndex);
		TextureFilter convertFilter(int filter);
		TextureWrap convertWrap(int wrap);
		void clearAnimations(ModelComponent& model);

		// Terrain
		size_t getTerrainGridArraySize(int rootGridSize, int levels);
		float getTerrainHeight(TerrainComponent& terrain, float x, float y);
		float maxTerrainHeightArea(TerrainComponent& terrain, float x, float z, float w, float h);
		float minTerrainHeightArea(TerrainComponent& terrain, float x, float z, float w, float h);
		TerrainNodeIndex createPlaneNodeBuffer(TerrainComponent& terrain, int width, int height, int widthSegments, int heightSegments);
		void createTerrain(TerrainComponent& terrain);
		void createTerrainNode(TerrainComponent& terrain, float x, float y, float size, int lodDepth);

	public:
		MeshSystem(Scene* scene);
		virtual ~MeshSystem();

		void createPlane(Entity entity, float width=1, float depth=1, unsigned int tiles=1);
		void createBox(Entity entity, float width=1, float height=1, float depth=1, unsigned int tiles=1);
		void createSphere(Entity entity, float radius=1, unsigned int slices=36, unsigned int stacks=18);
		void createCylinder(Entity entity, float baseRadius=1, float topRadius=1, float height=2, unsigned int slices=36, unsigned int stacks=18);
		void createTorus(Entity entity, float radius=1, float ringRadius=0.5, unsigned int sides=36, unsigned int rings=16);
		bool loadGLTF(Entity entity, std::string filename);
		bool loadOBJ(Entity entity, std::string filename);

		void destroyModel(ModelComponent& model);

		bool createOrUpdateSprite(SpriteComponent& sprite, MeshComponent& mesh);
		bool createOrUpdateTerrain(TerrainComponent& terrain);
		bool createOrUpdateMeshPolygon(MeshPolygonComponent& polygon, MeshComponent& mesh);
		bool createOrUpdateTilemap(TilemapComponent& tilemap, MeshComponent& mesh);

		virtual void load();
		virtual void destroy();
        virtual void update(double dt);
		virtual void draw();

		virtual void entityDestroyed(Entity entity);
	};

}

#endif //MESHSYSTEM_H