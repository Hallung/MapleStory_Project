MapleStory Project

커스텀 엔진 기반으로 제작한 2D 메이플스토리 모작 프로젝트입니다.

엔진 설계부터 에디터, 게임 플레이까지 직접 구현했습니다.

팀 구성
* 박정빈(JB): Engine / System
* 김휘재(HJ): Content / Gameplay

기술 스택
* C++
* DirectX
* SimpleMath
* ImGui
* FMOD

주요 구현

Engine (JB)
* Component 기반 구조
  * Object + Component 설계
  * 런타임 컴포넌트 추가/제거 지원
* TileMap & Editor
  * Instancing 기반 타일 렌더링
  * Grid ↔ World 좌표 변환
  * XML 기반 Save / Load
  * ImGui 기반 맵 에디터
  * Chain 데이터 Editor 연동 및 생성 기능 구현
* Camera 시스템 (디버그 / 릴리즈)
* FMOD 사운드 시스템 적용

Content (HJ)
* 캐릭터 & 몬스터 시스템 구현
  * State / Ability / Animator
  * RigidBody / Collider
  * HitEvents (충돌 이벤트 처리)
* 플레이어 컨트롤러 (이동 / 점프 / 공격)
* 몬스터 AI (BT 기반)
* 피격 및 넉백 시스템 개선
  * 공중 넉백 보정 / 감쇠(Damping)
  * 넉백 거리 일관성 유지
* 물리 엔진 Chain 구조를 커스텀 Collider 시스템에 통합
* 이펙트 시스템 (PlayEffect)
* 스프라이트 제작

공동 작업
* 렌더링 파이프라인 및 바인딩 구조
  (Mesh / Material / MeshRenderer / ShaderManager)
* Physics 시스템 (PhysicsManager)
* Collision 시스템 개선
* ObjectFactory 및 핵심 Manager 클래스
  (Time / Input / Texture 등)
* PathUtils 기반 리소스 경로 관리 및 직렬화 처리
* 윈도우 및 그래픽 초기화
* 협업 기반 디버깅 및 시스템 안정화

Links

* Repository
https://github.com/Hallung/MapleStory_Project

* Resource
https://drive.google.com/file/d/1vbsDHMk6xF-P5qeCsBY7944m8W7r7hp6/view?usp=drive_link

* YouTube
https://youtu.be/Dj2aS_gCm6g
